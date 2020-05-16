#include <stdio.h>

#include "dct.h"

int main(void) {
    // test DTC sur une matrice 8x8 donnée
    int8_t matrice_spatiale[8][8] = {{-16, 11, 10, 16, 24, 40, 51, 61},
                                     {12, 12, 14, 19, 26, 58, 60, 55},
                                     {14, 13, 16, 24, 40, 57, 69, 56},
                                     {14, 17, 22, 29, 51, 87, 80, 62},
                                     {18, 22, 37, 56, 68, 109, 103, 77},
                                     {24, 35, 55, 64, 81, 104, 113, 92},
                                     {49, 64, 78, 87, 103, 121, 120, 101},
                                     {72, 92, 95, 98, 112, 100, 103, 99}};

    printf("Matrice dans le domaine spatial");
    for (uint8_t x = 0; x < 8; x++) {
        for (uint8_t y = 0; y < 8; y++) {
            printf("%d\t", matrice_spatiale[y][x]);
        }
        printf("\n");
    }

    int16_t dct_buf[8][8];
    dct(matrice_spatiale, dct_buf);

    printf("Matrice dans le domaine fréquentiel");
    for (uint8_t x = 0; x < 8; x++) {
        for (uint8_t y = 0; y < 8; y++) {
            printf("%d\t", dct_buf[y][x]);
        }
        printf("\n");
    }
}
