#include <stdio.h>
#include <stdlib.h>

#include "dct.h"

int main(void) {
    /* Static 2D array */
    int16_t static_matrice_spatiale[8][8] = {{16, 11, 10, 16, 24, 40, 51, 61},
                                             {12, 12, 14, 19, 26, 58, 60, 55},
                                             {14, 13, 16, 24, 40, 57, 69, 56},
                                             {14, 17, 22, 29, 51, 87, 80, 62},
                                             {18, 22, 37, 56, 68, 109, 103, 77},
                                             {24, 35, 55, 64, 81, 104, 113, 92},
                                             {49, 64, 78, 87, 103, 121, 120, 101},
                                             {72, 92, 95, 98, 112, 100, 103, 99}};

    /* Dynamic 2D array initialisation */
    int16_t **matrice_spatiale = malloc(8 * sizeof *matrice_spatiale);
    if (matrice_spatiale == NULL) exit(EXIT_FAILURE);
    for (size_t i = 0; i < 8; i++) {
        matrice_spatiale[i] = malloc(8 * sizeof *matrice_spatiale[i]);
        if (matrice_spatiale[i] == NULL) exit(EXIT_FAILURE);
        matrice_spatiale[i] = static_matrice_spatiale[i];
    }

    printf("Matrice dans le domaine spatial\n");
    for (uint8_t x = 0; x < 8; x++) {
        for (uint8_t y = 0; y < 8; y++) {
            printf("%d\t", matrice_spatiale[y][x]);
        }
        printf("\n");
    }

    int16_t **dct_buf = malloc(8 * sizeof *dct_buf);
    if (dct_buf == NULL) exit(EXIT_FAILURE);
    for (size_t i = 0; i < 8; i++) {
        dct_buf[i] = malloc(8 * sizeof *dct_buf[i]);
        if (dct_buf[i] == NULL) exit(EXIT_FAILURE);
    }
    dct(matrice_spatiale, dct_buf);

    printf("Matrice dans le domaine fréquentiel\n");
    for (uint8_t x = 0; x < 8; x++) {
        for (uint8_t y = 0; y < 8; y++) {
            printf("%d\t", dct_buf[y][x]);
        }
        printf("\n");
    }
}
