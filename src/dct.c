#include "dct.h"

#include <stddef.h>
#include <stdint.h>

/* 
    Pour tester ses valeurs : https://asecuritysite.com/comms/dct2

    On a un écart corrigé en appliquant -1.

    Liens à consulter:
        https://www-ljk.imag.fr/membres/Valerie.Perrier/SiteWeb/node9.html

    On n'invente rien ici. On utilise l'algorithme de Arai, Agui, et Nakajima (AAC) 
    pour une transformée de 8 points. Tout d'abord sur les lignes puis sur les colonnes.
    En utilisant la représentation d'un nombre en virgule fixe (voir https://github.com/prtsh/aan_dct/blob/master/8point_DCT_AAN.c).

    Implémentation de 
    https://opencores.org/websvn/filedetails?repname=dct_idct&path=%2Fdct_idct%2Ftrunk%2Fdct%2FDoc%2Fdct_um.pdf
    
    Simplifié à 3 phases.
    Les phases e, f, sa sont incluses dans les 3 premières.
    On transforme les produits de cosinus en sommes:
        2cos(pi/4)cos(pi/8) = cos(3pi/8) + cos(pi/8) (f2 et f3 au signe près)
        2cos(pi/4)cos(pi/16) = cos(5pi/16) + cos(3pi/16)
        2cos(pi/4)cos(3pi/16) = cos(7pi/16) + cos(3pi/16)

    The output array of DCT coefficients contains integers; 
    these can range from -1024 to 1023.
*/
void dct(int8_t input[8][8], int16_t output[8][8]) {
    size_t i;
    int tmp[8][8];

    /* Approximations entières des valeurs des cosinus */
    static const int cos1 = 1004, /* cos(pi/16) << 10 */
                     cos7 = 200, /* cos(7pi/16) << 10 */
                     cos3 = 851, /* cos(3pi/16) << 10 */
                     cos5 = 569, /* cos(5pi/16) << 10 */
                     racine2cos6 = 554, /* sqrt(2)*cos(6pi/16) << 10 */
                     racine2cos2 = 1337, /* sqrt(2)*cos(2pi/16) << 10 */
                     racine2 = 181; /* sqrt(2) << 7 */

    int a0, a1, a2, a3, a4, a5, a6, a7, a8;

    /* opérations sur les lignes */
    for (i = 0; i < 8; i++) {
        a0 = (int)(input[0][i]);
        a1 = (int)(input[1][i]);
        a2 = (int)(input[2][i]);
        a3 = (int)(input[3][i]);
        a4 = (int)(input[4][i]);
        a5 = (int)(input[5][i]);
        a6 = (int)(input[6][i]);
        a7 = (int)(input[7][i]);

        /* Etape 1: correspond aux b
        on remplace :
            b7 par a0,
            b3 par a1,
            b6 par a2,
            b2 par a3,
            b5 par a5,
            b4 par a6,
            b1 par a7,
            b0 par a8,
        on réorganise l'ordre des opérations pour éviter les variables intermédiaires,
        */
        a8 = a7 + a0;
        a0 -= a7;
        a7 = a1 + a6;
        a1 -= a6;
        a6 = a2 + a5;
        a2 -= a5;
        a5 = a3 + a4;
        a3 -= a4;

        /* Etape 2 
        on remplace :
            c0 par a4,
            c3 par a5,
            c4 par a8,
            c1 par a7,
        */
        a4 = a8 + a5;
        a8 -= a5;
        a5 = a7 + a6;
        a7 -= a6;
        a6 = cos1 * (a1 + a2);
        a2 = (-cos7 - cos1) * a2 + a6;
        a1 = (cos7 - cos1) * a1 + a6;
        a6 = cos3 * (a0 + a3);
        a3 = (-cos5 - cos3) * a3 + a6;
        a0 = (cos5 - cos3) * a0 + a6;

        /* Etape 3 
        on remplace :
            d0 par a6,
            d1 par a4,
            d3 par a5 / racine2cos6,
            
            f7 par a5,
            f3 par a2,
        */
        a6 = a4 + a5;
        a4 -= a5;
        a5 = racine2cos6 * (a7 + a8);
        a7 = (-racine2cos2 - racine2cos6) * a7 + a5; // f3
        a8 = (racine2cos2 - racine2cos6) * a8 + a5; // f2

        a5 = a0 + a2;
        a0 -= a2;
        a2 = a3 + a1;
        a3 -= a1;

        /* Etape 4 */
        tmp[i][0] = a6;
        tmp[i][4] = a4;
        tmp[i][2] = (a8 >> 10);  // redécallage de 10
        tmp[i][6] = (a7 >> 10);
        tmp[i][7] = ((a2 - a5) >> 10);
        tmp[i][1] = ((a2 + a5) >> 10);
        tmp[i][3] = ((a3 * racine2) >> 17);  // redécallage de 10 + 7
        tmp[i][5] = ((a0 * racine2) >> 17);
    }

    /* opérations sur les colonnes */
    for (i = 0; i < 8; i++) {
        a0 = tmp[0][i];
        a1 = tmp[1][i];
        a2 = tmp[2][i];
        a3 = tmp[3][i];
        a4 = tmp[4][i];
        a5 = tmp[5][i];
        a6 = tmp[6][i];
        a7 = tmp[7][i];

        /* Etape 1 */
        a8 = a7 + a0;
        a0 -= a7;
        a7 = a1 + a6;
        a1 -= a6;
        a6 = a2 + a5;
        a2 -= a5;
        a5 = a3 + a4;
        a3 -= a4;

        /* Etape 2 */
        a4 = a8 + a5;
        a8 -= a5;
        a5 = a7 + a6;
        a7 -= a6;
        a6 = cos1 * (a1 + a2);
        a2 = (-cos7 - cos1) * a2 + a6;
        a1 = (cos7 - cos1) * a1 + a6;
        a6 = cos3 * (a0 + a3);
        a3 = (-cos5 - cos3) * a3 + a6;
        a0 = (cos5 - cos3) * a0 + a6;

        /* Etape 3 */
        a6 = a4 + a5;
        a4 -= a5;
        a5 = racine2cos6 * (a7 + a8);
        a7 = (-racine2cos2 - racine2cos6) * a7 + a5;
        a8 = (racine2cos2 - racine2cos6) * a8 + a5;
        a5 = a0 + a2;
        a0 -= a2;
        a2 = a3 + a1;
        a3 -= a1;

        /* Etape 4 */
        /* La racine de deux change les décallages */
        output[0][i] = (int16_t)((a6 + 4) >> 3);  // redécallage de 0 + (10 - 7), ajout de 1/4 pour un meilleur résultat
        output[4][i] = (int16_t)((a4 + 4) >> 3);
        output[2][i] = (int16_t)((a8 + 4096) >> 13);  // redécallage de 10 + (10 - 7), ajout de 1/2
        output[6][i] = (int16_t)((a7 + 4096) >> 13);
        output[7][i] = (int16_t)((a2 - a5 + 4096) >> 13);
        output[1][i] = (int16_t)((a2 + a5 + 4096) >> 13);
        output[3][i] = (int16_t)(((a3 >> 8) * racine2 + 2048) >> 12);  // redécallage de 17 - 8 + (10 - 7), ajout de 1/2
        output[5][i] = (int16_t)(((a0 >> 8) * racine2 + 2048) >> 12);
    }
}