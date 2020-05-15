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

// remplacer les zera par des c pour respecter la syntaxe
float** DTC(float matrice_domaine_spatial[8][8], matrice_cosinus_dct** matrices_dtc, uint8_t n) {
    float** matrice_domaine_frequentiel;
    matrice_domaine_frequentiel = (float**)malloc(n * sizeof(float*));
    for (uint32_t k = 0; k < n; k++) {
        matrice_domaine_frequentiel[k] = (float*)malloc(n * sizeof(float));
    }
    float zeta_i;
    float zeta_j;
    float somme;
    for (uint8_t i = 0; i < n; i++) {
        for (uint8_t j = 0; j < n; j++) {
            zeta_i = 1;
            zeta_j = 1;
            if (i == 0) {
                zeta_i = (float)(1 / sqrt(2));
            }
            if (j == 0) {
                zeta_j = (float)(1 / sqrt(2));
            }
            somme = 0;
            for (uint8_t x = 0; x < n; x++) {
                for (uint8_t y = 0; y < n; y++) {
                    somme += matrice_domaine_spatial[x][y] * (matrices_dtc[i][j]).matrice[x][y];
                }
            }
            matrice_domaine_frequentiel[i][j] = (2 / (float)n) * zeta_i * zeta_j * somme;
        }
    }
    return matrice_domaine_frequentiel;
}

/*
Main que j'ai utilisé pour tester la bonne compilation et vérifier que tout marche.
On pourra revoir les types ensemble  mais globalement j'obtiens les bonnes valeurs
de dtc à partir d'une matrice spatiale 8x8 et retournées dans une matrice 8x8 frequentielle.
*/

//int main (void) {
// test pour les matrices de dtc -- ça fonctionne bien
/*
  matrice_cosinus_dct** matrices_dtc = creer_matrices_dtc(8);
  for (uint8_t i=0; i<8;i++) {
    for (uint8_t j = 0; j<8;j++) {
      printf("matrice %i %i : \n", i, j);
      for (uint8_t x = 0; x<8; x++) {
        for (uint8_t y = 0; y<8; y++) {
          printf("%f\t", (matrices_dtc[i][j]).matrice[x][y]);
        }
        printf("\n");
      }
    }
  }
  // fin du test
  */

//test pour la dtc sur un exemple -- j'obtiens les bonnes valeurs
/*
  float matrice_spatiale[8][8] = {{16,  11,  10,  16,  24,  40,  51,  61},
  {12,  12,  14,  19,  26,  58,  60,  55},
  {14,  13,  16,  24,  40,  57,  69,  56},
  {14,  17,  22,  29,  51,  87,  80,  62},
  {18,  22,  37,  56,  68, 109, 103,  77},
  {24,  35,  55,  64,  81, 104, 113,  92},
  {49,  64,  78,  87, 103, 121, 120, 101},
  {72,  92,  95,  98, 112, 100, 103,  99}};
  float** matrice_freq;
  matrice_freq = DTC(matrice_spatiale, matrices_dtc, 8);
  printf("matrice freq : \n");
  for (uint8_t x = 0; x<8; x++) {
    for (uint8_t y = 0; y<8; y++) {
      printf("%f\t", matrice_freq[x][y]);
    }
    printf("\n");
  }
  // fin du test
  */
//}
