#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

/* type: int16_t**
 * rtype: void
 * Affiche une data unit de taille 8x8 en hexa
 */
static void afficher_data_unit(int16_t** data_unit) {
    printf("Data Unit\n");
    for (size_t i = 0; i < 8; i++) {
        for (size_t j = 0; j < 8; j++) {
            printf("%02hhX ", data_unit[i][j]);
        }
        printf("\n");
    }
}

void process_Y(int16_t** Y_mcu, uint8_t hy, uint8_t vy, int16_t** data_unit) {
    /* Traitement des blocs Y */
    printf("%d %d\n", vy, hy);
    for (size_t v = 0; v < vy; v++) {
        for (size_t h = 0; h < hy; h++) {
            for (size_t i = 0; i < 8; i++) {
                for (size_t j = 0; j < 8; j++) {
                    data_unit[i][j] = Y_mcu[i + 8 * v][j + 8 * h];
                }
            }
            /* Encodage de data_unit */
            afficher_data_unit(data_unit);
        }
    }
}

void process_chroma(int16_t** chroma_mcu, uint8_t hy, uint8_t vy, uint8_t h_chroma, uint8_t v_chroma, int16_t** data_unit) {
    /* Traitement des blocs Cb / Cr avec échantillonnage horizontal */
    // printf("%d \n", vy);
    // printf("%d %d\n", h_chroma, v_chroma);
    float remainder = 0;
    uint8_t chroma_value = 0;
    uint8_t h_div = (hy / h_chroma);
    uint8_t v_div = (vy / v_chroma);
    // printf("%d %d\n", h_div, v_div);
    /* On boucle dans l'ordre des DU à traiter */
    for (size_t v = 0; v < v_chroma; v++) {
        for (size_t h = 0; h < h_chroma; h++) {
            printf("%ld, %ld\n", v, h);
            /* Echantillonnage de la chrominance */
            for (size_t i = 0; i < 8; i++) {
                for (size_t j = 0; j < 8; j++) {
                    for (size_t k = 0; k < h_div; k++) {
                        for (size_t l = 0; l < v_div; l++) {
                            chroma_value = (uint8_t)chroma_mcu[v_div * i + l + 8 * v][h_div * j + k + 8 * h];
                            data_unit[i][j] = (uint8_t)(data_unit[i][j] + chroma_value / (h_div * v_div));
                            remainder += (float)(chroma_value % (h_div * v_div));
                        }
                    }
                    data_unit[i][j] = (int16_t)(data_unit[i][j] + remainder / (float)(h_div * v_div));
                    remainder = 0;
                }
            }
            /* Traitement des DUs */
            afficher_data_unit(data_unit);
            // à faire pdt l'écriture dans le bitstream.
            /* Mise à zéro de data_unit */
            for (size_t i = 0; i < 8; i++) {
                for (size_t j = 0; j < 8; j++) {
                    data_unit[i][j] = 0;
                }
            }
        }
    }
}
