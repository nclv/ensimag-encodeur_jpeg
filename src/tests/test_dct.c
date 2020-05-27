#include <stdio.h>
#include <stdlib.h>

#include "dct.h"

int main(void) {
    /* Static 2D array */
    int16_t static_spatial_matrix[8][8] = {{16, 11, 10, 16, 24, 40, 51, 61},
                                           {12, 12, 14, 19, 26, 58, 60, 55},
                                           {14, 13, 16, 24, 40, 57, 69, 56},
                                           {14, 17, 22, 29, 51, 87, 80, 62},
                                           {18, 22, 37, 56, 68, 109, 103, 77},
                                           {24, 35, 55, 64, 81, 104, 113, 92},
                                           {49, 64, 78, 87, 103, 121, 120, 101},
                                           {72, 92, 95, 98, 112, 100, 103, 99}};

    /* Dynamic 2D array initialisation */
    int16_t **spatial_matrix = malloc(8 * sizeof *spatial_matrix);
    if (spatial_matrix == NULL) exit(EXIT_FAILURE);
    for (size_t i = 0; i < 8; i++) {
        spatial_matrix[i] = static_spatial_matrix[i];
    }

    printf("\nMatrice dans le domaine spatial\n");
    for (uint8_t x = 0; x < 8; x++) {
        for (uint8_t y = 0; y < 8; y++) {
            printf("%d\t", spatial_matrix[y][x]);
        }
        printf("\n");
    }

    int16_t dct_buf[8][8] = {0};
    dct(spatial_matrix, dct_buf);

    printf("\nMatrice dans le domaine fréquentiel\n");
    for (uint8_t x = 0; x < 8; x++) {
        for (uint8_t y = 0; y < 8; y++) {
            printf("%d\t", dct_buf[y][x]);
        }
        printf("\n");
    }

    offset(spatial_matrix);

    printf("\nMatrice dans le domaine spatial\n");
    for (uint8_t x = 0; x < 8; x++) {
        for (uint8_t y = 0; y < 8; y++) {
            printf("%d\t", spatial_matrix[y][x]);
        }
        printf("\n");
    }

    dct(spatial_matrix, dct_buf);

    printf("\nMatrice dans le domaine fréquentiel\n");
    for (uint8_t x = 0; x < 8; x++) {
        for (uint8_t y = 0; y < 8; y++) {
            printf("%d\t", dct_buf[y][x]);
        }
        printf("\n");
    }

    /* Dynamic 2D array freeing */
    free(spatial_matrix);
}
