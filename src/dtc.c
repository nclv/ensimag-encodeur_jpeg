#include "dtc.h"

#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

matrice_cosinus_dct* creer_matrice_i_j(uint8_t i, uint8_t j, uint8_t n) {
    matrice_cosinus_dct* matrice_i_j = (matrice_cosinus_dct*)malloc(sizeof(matrice_cosinus_dct));
    matrice_i_j->matrice = (float**)malloc(n * sizeof(float*));
    for (uint8_t k = 0; k < n; k++) {
        matrice_i_j->matrice[k] = (float*)malloc(n * sizeof(float));
    }
    for (uint8_t x = 0; x < n; x++) {
        for (uint8_t y = 0; y < n; y++) {
            matrice_i_j->matrice[x][y] = cosf(((2 * x + 1) * i * M_PI) / (2 * n)) * cosf(((2 * y + 1) * j * M_PI) / (2 * n));
            // on remplit la matrice des cosinus pour la dtc d'indices (i,j)
        }
    }
    return matrice_i_j;
}

matrice_cosinus_dct** creer_matrices_dtc(uint8_t n) {
    matrice_cosinus_dct** matrices_dtc;
    matrices_dtc = (matrice_cosinus_dct**)malloc(n * sizeof(matrice_cosinus_dct*));
    for (uint32_t i = 0; i < n; i++) {
        matrices_dtc[i] = (matrice_cosinus_dct*)malloc(n * sizeof(matrice_cosinus_dct));
    }
    for (uint8_t i = 0; i < n; i++) {
        for (uint8_t j = 0; j < n; j++) {
            // on remplit la matrice des matrices de cosinus utiles pour la dtc
            matrices_dtc[i][j] = *creer_matrice_i_j(i, j, n);
        }
    }
    return matrices_dtc;
}

float** DTC(float matrice_domaine_spatial[8][8], matrice_cosinus_dct** matrices_dtc, uint8_t n) {
    float** matrice_domaine_frequentiel;
    matrice_domaine_frequentiel = (float**)malloc(n * sizeof(float*));
    for (uint32_t k = 0; k < n; k++) {
        matrice_domaine_frequentiel[k] = (float*)malloc(n * sizeof(float));
    }
    float c_i;
    float c_j;
    float somme;
    for (uint8_t i = 0; i < n; i++) {
        for (uint8_t j = 0; j < n; j++) {
            c_i = 1;
            c_j = 1;
            if (i == 0) {
                c_i = (float)(1 / sqrt(2));
            }
            if (j == 0) {
                c_j = (float)(1 / sqrt(2));
            }
            somme = 0;
            for (uint8_t x = 0; x < n; x++) {
                for (uint8_t y = 0; y < n; y++) {
                    somme += matrice_domaine_spatial[x][y] * (matrices_dtc[i][j]).matrice[x][y];
                }
            }
            matrice_domaine_frequentiel[i][j] = (2 / (float)n) * c_i * c_j * somme;
        }
    }
    return matrice_domaine_frequentiel;
}
