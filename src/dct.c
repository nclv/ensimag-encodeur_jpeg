#include "dct.h"

#include <stdint.h>
#include <stddef.h>

/* 
    Pour tester ses valeurs : https://asecuritysite.com/comms/dct2

    On a un écart corrigé en appliquant -1.

    Liens à consulter:
        https://www-ljk.imag.fr/membres/Valerie.Perrier/SiteWeb/node9.html

    Implémentation de 
    https://opencores.org/websvn/filedetails?repname=dct_idct&path=%2Fdct_idct%2Ftrunk%2Fdct%2FDoc%2Fdct_um.pdf
    
    On n'invente rien ici. On utilise l'algorithme de Arai, Agui, et Nakajama (AAC) 
    pour une transformée de 8 points. Tout d'abord sur les lignes puis sur les colonnes.
    En utilisant la représentation d'un nombre en virgule fixe.

    The output array of DCT coefficients contains integers; 
    these can range from -1024 to 1023.
*/
void dct(int8_t input[8][8], int16_t output[8][8]) {
    size_t i;
    int rows[8][8];

    /* Approximations des valeurs des cosinus */
    static const int cos1 = 1004 /* cos(pi/16) << 10 */,
                     cos7 = 200 /* cos(7pi/16) */,
                     cos3 = 851 /* cos(3pi/16) << 10 */,
                     cos5 = 569 /* cos(5pi/16) << 10 */,
                     racine2cos6 = 554 /* sqrt(2)*cos(6pi/16) << 10 */,
                     racine2sin6 = 1337 /* sqrt(2)*cos(2pi/16) << 10 */,
                     racine2 = 181; /* sqrt(2) << 7*/

    int x0, x1, x2, x3, x4, x5, x6, x7, x8;

    /* opérations sur les lignes */
    for (i = 0; i < 8; i++) {
        x0 = (int)(input[0][i]);
        x1 = (int)(input[1][i]);
        x2 = (int)(input[2][i]);
        x3 = (int)(input[3][i]);
        x4 = (int)(input[4][i]);
        x5 = (int)(input[5][i]);
        x6 = (int)(input[6][i]);
        x7 = (int)(input[7][i]);

        /* Stage 1 */
        x8 = x7 + x0;
        x0 -= x7;
        x7 = x1 + x6;
        x1 -= x6;
        x6 = x2 + x5;
        x2 -= x5;
        x5 = x3 + x4;
        x3 -= x4;

        /* Stage 2 */
        x4 = x8 + x5;
        x8 -= x5;
        x5 = x7 + x6;
        x7 -= x6;
        x6 = cos1 * (x1 + x2);
        x2 = (-cos7 - cos1) * x2 + x6;
        x1 = (cos7 - cos1) * x1 + x6;
        x6 = cos3 * (x0 + x3);
        x3 = (-cos5 - cos3) * x3 + x6;
        x0 = (cos5 - cos3) * x0 + x6;

        /* Stage 3 */
        x6 = x4 + x5;
        x4 -= x5;
        x5 = racine2cos6 * (x7 + x8);
        x7 = (-racine2sin6 - racine2cos6) * x7 + x5;
        x8 = (racine2sin6 - racine2cos6) * x8 + x5;
        x5 = x0 + x2;
        x0 -= x2;
        x2 = x3 + x1;
        x3 -= x1;

        /* Stage 4 and output */
        rows[i][0] = x6;
        rows[i][4] = x4;
        rows[i][2] = (x8 >> 10); // redécallage de 10
        rows[i][6] = (x7 >> 10);
        rows[i][7] = ((x2 - x5) >> 10);
        rows[i][1] = ((x2 + x5) >> 10);
        rows[i][3] = ((x3 * racine2) >> 17); // redécallage de 10 + 7
        rows[i][5] = ((x0 * racine2) >> 17);
    }

    /* opérations sur les colonnes */
    for (i = 0; i < 8; i++) {
        x0 = rows[0][i];
        x1 = rows[1][i];
        x2 = rows[2][i];
        x3 = rows[3][i];
        x4 = rows[4][i];
        x5 = rows[5][i];
        x6 = rows[6][i];
        x7 = rows[7][i];

        /* Stage 1 */
        x8 = x7 + x0;
        x0 -= x7;
        x7 = x1 + x6;
        x1 -= x6;
        x6 = x2 + x5;
        x2 -= x5;
        x5 = x3 + x4;
        x3 -= x4;

        /* Stage 2 */
        x4 = x8 + x5;
        x8 -= x5;
        x5 = x7 + x6;
        x7 -= x6;
        x6 = cos1 * (x1 + x2);
        x2 = (-cos7 - cos1) * x2 + x6;
        x1 = (cos7 - cos1) * x1 + x6;
        x6 = cos3 * (x0 + x3);
        x3 = (-cos5 - cos3) * x3 + x6;
        x0 = (cos5 - cos3) * x0 + x6;

        /* Stage 3 */
        x6 = x4 + x5;
        x4 -= x5;
        x5 = racine2cos6 * (x7 + x8);
        x7 = (-racine2sin6 - racine2cos6) * x7 + x5;
        x8 = (racine2sin6 - racine2cos6) * x8 + x5;
        x5 = x0 + x2;
        x0 -= x2;
        x2 = x3 + x1;
        x3 -= x1;

        /* Stage 4 and output */
        /* La racine de deux change les décallages */
        output[0][i] = (int16_t)((x6 + 4) >> 3); // redécallage de 0 + (10 - 7), ajout de 1/4 pour un meilleur résultat
        output[4][i] = (int16_t)((x4 + 4) >> 3);
        output[2][i] = (int16_t)((x8 + 4096) >> 13); // redécallage de 10 + (10 - 7), ajout de 1/2
        output[6][i] = (int16_t)((x7 + 4096) >> 13);
        output[7][i] = (int16_t)((x2 - x5 + 4096) >> 13);
        output[1][i] = (int16_t)((x2 + x5 + 4096) >> 13);
        output[3][i] = (int16_t)(((x3 >> 8) * racine2 + 2048) >> 12); // redécallage de 17 - 8 + (10 - 7), ajout de 1/2
        output[5][i] = (int16_t)(((x0 >> 8) * racine2 + 2048) >> 12);
    }
}