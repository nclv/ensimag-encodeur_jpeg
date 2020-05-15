#include "dtc.h"

#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define M_PI 3.14159265358979323846

/* 
    Pour tester ses valeurs : https://asecuritysite.com/comms/dct2

    On a un écart corrigé en appliquant -1.

    Liens à consulter:
        https://www-ljk.imag.fr/membres/Valerie.Perrier/SiteWeb/node9.html

    Implémentation de 
    https://opencores.org/websvn/filedetails?repname=dct_idct&path=%2Fdct_idct%2Ftrunk%2Fdct%2FDoc%2Fdct_um.pdf
    
    On n'invente rien ici. On utilise l'algorithme de Arai, Agui, et Nakajama (AAC) 
    pour une transformée de 8 points. Tout d'abord sur les lignes puis sur les colonnes.
*/
void dct(double input[8][8], int output[8][8]) {
    int i;
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
        output[0][i] = ((x6 + 4) >> 3); // redécallage de 0 + (10 - 7), ajout de 1/4 pour un meilleur résultat
        output[4][i] = ((x4 + 4) >> 3);
        output[2][i] = ((x8 + 4096) >> 13); // redécallage de 10 + (10 - 7), ajout de 1/2
        output[6][i] = ((x7 + 4096) >> 13);
        output[7][i] = ((x2 - x5 + 4096) >> 13);
        output[1][i] = ((x2 + x5 + 4096) >> 13);
        output[3][i] = (((x3 >> 8) * racine2 + 2048) >> 12); // redécallage de 17 - 8 + (10 - 7), ajout de 1/2
        output[5][i] = (((x0 >> 8) * racine2 + 2048) >> 12);
    }
}

matrice_cosinus_dct* creer_matrice_i_j(uint8_t i, uint8_t j, uint8_t n) {
    matrice_cosinus_dct* matrice_i_j = malloc(sizeof(matrice_cosinus_dct));
    matrice_i_j->matrice = malloc(n * sizeof(double*));
    for (uint8_t k = 0; k < n; k++) {
        matrice_i_j->matrice[k] = malloc(n * sizeof(double));
    }
    for (uint8_t x = 0; x < n; x++) {
        for (uint8_t y = 0; y < n; y++) {
            matrice_i_j->matrice[x][y] = cos(((2 * x + 1) * i * M_PI) / (2 * n)) * cos(((2 * y + 1) * j * M_PI) / (2 * n));
            // on remplit la matrice des cosinus pour la dtc d'indices (i,j)
        }
    }
    return matrice_i_j;
}

matrice_cosinus_dct** creer_matrices_dtc(uint8_t n) {
    matrice_cosinus_dct** matrices_dtc;
    matrices_dtc = malloc(n * sizeof(matrice_cosinus_dct*));
    for (uint32_t i = 0; i < n; i++) {
        matrices_dtc[i] = malloc(n * sizeof(matrice_cosinus_dct));
    }
    for (uint8_t i = 0; i < n; i++) {
        for (uint8_t j = 0; j < n; j++) {
            // on remplit la matrice des matrices de cosinus utiles pour la dtc
            matrices_dtc[i][j] = *creer_matrice_i_j(i, j, n);
        }
    }
    return matrices_dtc;
}

double** DTC(double matrice_domaine_spatial[8][8], matrice_cosinus_dct** matrices_dtc, uint8_t n) {
    double** matrice_domaine_frequentiel;
    matrice_domaine_frequentiel = malloc(n * sizeof(double*));
    for (uint32_t k = 0; k < n; k++) {
        matrice_domaine_frequentiel[k] = malloc(n * sizeof(double));
    }
    double c_i;
    double c_j;
    double somme;
    for (uint8_t i = 0; i < n; i++) {
        for (uint8_t j = 0; j < n; j++) {
            c_i = 1;
            c_j = 1;
            if (i == 0) {
                c_i = (1 / sqrt(2));
            }
            if (j == 0) {
                c_j = (1 / sqrt(2));
            }
            somme = 0;
            for (uint8_t x = 0; x < n; x++) {
                for (uint8_t y = 0; y < n; y++) {
                    somme += matrice_domaine_spatial[x][y] * (matrices_dtc[i][j]).matrice[x][y];
                }
            }
            matrice_domaine_frequentiel[i][j] = (2 / (double)n) * c_i * c_j * somme;
        }
    }
    return matrice_domaine_frequentiel;
}
