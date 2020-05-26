#include <stdio.h>
#include <stdlib.h>

#include "qtables.h"
#include "quantification.h"

int main(void) {
    int16_t static_freq_matrix[8][8] = {{25, 11, 10, 16, 24, 40, 51, 30},
                                        {12, 12, 14, 19, 26, 58, 60, 55},
                                        {14, 13, 16, 24, 40, 57, 69, 56},
                                        {14, 17, 22, 29, 51, 87, 80, 62},
                                        {18, 22, 37, 56, 68, 109, 103, 77},
                                        {24, 35, 55, 64, 81, 104, 113, 92},
                                        {49, 64, 78, 87, 103, 121, 120, 101},
                                        {72, 92, 95, 98, 112, 100, 103, 99}};

    /* Dynamic 2D array initialisation */
    int16_t **freq_matrix = malloc(8 * sizeof *freq_matrix);
    if (freq_matrix == NULL) exit(EXIT_FAILURE);
    for (size_t i = 0; i < 8; i++) {
        freq_matrix[i] = malloc(8 * sizeof *freq_matrix[i]);
        if (freq_matrix[i] == NULL) exit(EXIT_FAILURE);
        freq_matrix[i] = static_freq_matrix[i];
    }

    quantifier(freq_matrix, quantification_table_Y);
    printf("Quantification de Y\n");
    for (size_t i = 0; i < 8; i++) {
        for (size_t j = 0; j < 8; j++) {
            printf("%i | ", freq_matrix[i][j]);
        }
        printf("\n");
    }

    quantifier(freq_matrix, quantification_table_CbCr);
    printf("Quantification de Cb ou Cr\n");
    for (size_t i = 0; i < 8; i++) {
        for (size_t j = 0; j < 8; j++) {
            printf("%i | ", freq_matrix[i][j]);
        }
        printf("\n");
    }

    for (size_t i = 0; i < 8; i++) {
        free(freq_matrix[i]);
    }
    free(freq_matrix);
}
