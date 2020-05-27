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

    quantifier(static_freq_matrix, quantification_table_Y);
    printf("\nQuantification de Y\n");
    afficher_matrice_quantifiee(static_freq_matrix);

    quantifier(static_freq_matrix, quantification_table_CbCr);
    printf("\nQuantification de Cb ou Cr\n");
    afficher_matrice_quantifiee(static_freq_matrix);
}
