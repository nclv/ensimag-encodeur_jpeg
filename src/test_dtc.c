#include "dtc.h"

int main(void) {
    // test de la partie précalcul des cosinus pour la DTC
    matrice_cosinus_dct** matrices_dtc = creer_matrices_dtc(8);
    for (uint8_t i = 0; i < 8; i++) {
        for (uint8_t j = 0; j < 8; j++) {
            printf("matrice %i %i : \n", i, j);
            for (uint8_t x = 0; x < 8; x++) {
                for (uint8_t y = 0; y < 8; y++) {
                    printf("%f\t", (matrices_dtc[i][j]).matrice[x][y]);
                }
                printf("\n");
            }
        }
    }
    printf("\n");
    // test DTC sur une matrice 8x8 donnée
    double matrice_spatiale[8][8] = {{16, 11, 10, 16, 24, 40, 51, 61},
                                    {12, 12, 14, 19, 26, 58, 60, 55},
                                    {14, 13, 16, 24, 40, 57, 69, 56},
                                    {14, 17, 22, 29, 51, 87, 80, 62},
                                    {18, 22, 37, 56, 68, 109, 103, 77},
                                    {24, 35, 55, 64, 81, 104, 113, 92},
                                    {49, 64, 78, 87, 103, 121, 120, 101},
                                    {72, 92, 95, 98, 112, 100, 103, 99}};
    double** matrice_freq;
    matrice_freq = DTC(matrice_spatiale, matrices_dtc, 8);
    printf("matrice spatiale : \n");
    for (uint8_t x = 0; x < 8; x++) {
        for (uint8_t y = 0; y < 8; y++) {
            printf("%f\t", matrice_spatiale[x][y]);
        }
        printf("\n");
    }
    printf("matrice freq : \n");
    for (uint8_t x = 0; x < 8; x++) {
        for (uint8_t y = 0; y < 8; y++) {
            printf("%f\t", matrice_freq[x][y]);
        }
        printf("\n");
    }
}
