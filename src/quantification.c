#include "quantification.h"

#include <stddef.h>
#include <stdint.h>

void quantifier(int16_t **matrice_frequentiel_zigzag, uint8_t quantification_table[64]) {
    size_t inc = 0;
    for (size_t i = 0; i < 8; i++) {
        for (size_t j = 0; j < 8; j++) {
            matrice_frequentiel_zigzag[i][j] = (int16_t)(matrice_frequentiel_zigzag[i][j] / quantification_table[i + j + inc]);
        }
        inc += 7;
    }
}
