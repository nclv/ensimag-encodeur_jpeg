#include <stdint.h>

#include "zigzag.h"

int main(void) {
    // Matrice 8x8 de nombres aléatoirs entre 0 et 255
    uint8_t matrice[8][8];

    uint8_t nb = 0;

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            matrice[i][j] = nb;
            nb++;
            printf("%u ", matrice[i][j]);
        }
        printf("\n");
    }

    // Affichage zigzag de la matrice
    uint8_t *tableau = zigzag(matrice);
    int inc = 0;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            printf("%d ", tableau[i + j + inc]);
        }
        inc += 7;
        printf("\n");
    }
}
