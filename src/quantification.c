#include "quantification.h"

#include <inttypes.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

void quantifier(int16_t matrice_frequentiel_zigzag[8][8], uint8_t quantification_table[64]) {
    size_t inc = 0;
    for (size_t i = 0; i < 8; i++) {
        for (size_t j = 0; j < 8; j++) {
            matrice_frequentiel_zigzag[i][j] = (int16_t)(matrice_frequentiel_zigzag[i][j] / quantification_table[i + j + inc]);
        }
        inc += 7;
    }
}

void afficher_matrice_quantifiee(const int16_t matrice_quantifiee[8][8]) {
    printf("\nQuantification: \n");
    for (uint8_t k = 0; k < 8; k++) {
        for (uint8_t l = 0; l < 8; l++) {
            printf("%04" PRIx16 " ", (uint16_t)matrice_quantifiee[k][l]);
        }
        printf("\n");
    }
}
