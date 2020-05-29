#include <stdio.h>
#include <stdlib.h>

#include "htables.h"
#include "jpeg_writer.h"
#include "qtables.h"
#include "verbose.h"

bool verbose = true;

static void afficher_quantization_table(uint8_t *qtable) {
    for (size_t i = 0; i < 64; i += 8) {
        for (size_t j = 0; j < 8; j++) {
            printf("%x, ", qtable[i + j]);
        }
        printf("\n");
    }
}

int main(void) {
    jpeg *jpg = jpeg_create();

    jpeg_set_jpeg_filename(jpg, "output.jpeg");
    jpeg_set_nb_components(jpg, 3);
    uint8_t nb_components = jpeg_get_nb_components(jpg);
    printf("\nNombre de composantes: %d\n", nb_components);

    jpeg_set_quantization_table(jpg, Y, quantification_table_Y);
    jpeg_set_quantization_table(jpg, Cb, quantification_table_CbCr);

    printf("\nY_qtables\n");
    uint8_t *Y_qtables = jpeg_get_quantization_table(jpg, Y);
    afficher_quantization_table(Y_qtables);

    printf("\nCbCr_qtables\n");
    uint8_t *CbCr_qtables = jpeg_get_quantization_table(jpg, Cr);
    afficher_quantization_table(CbCr_qtables);

    printf("\nConstruction des tables de Huffman\n");
    huff_table *htable_Y_DC = huffman_table_build(htables_nb_symb_per_lengths[DC][Y], htables_symbols[DC][Y], htables_nb_symbols[DC][Y]);
    if (htable_Y_DC == NULL) {
        jpeg_destroy(jpg);
        exit(EXIT_FAILURE);
    }
    jpeg_set_huffman_table(jpg, DC, Y, htable_Y_DC);

    huff_table *htable_Y_AC = huffman_table_build(htables_nb_symb_per_lengths[AC][Y], htables_symbols[AC][Y], htables_nb_symbols[AC][Y]);
    if (htable_Y_AC == NULL) {
        jpeg_destroy(jpg);
        exit(EXIT_FAILURE);
    }
    jpeg_set_huffman_table(jpg, AC, Y, htable_Y_AC);

    huff_table *htable_CbCr_DC = huffman_table_build(htables_nb_symb_per_lengths[DC][Cb], htables_symbols[DC][Cb], htables_nb_symbols[DC][Cb]);
    if (htable_CbCr_DC == NULL) {
        jpeg_destroy(jpg);
        exit(EXIT_FAILURE);
    }
    jpeg_set_huffman_table(jpg, DC, Cb, htable_CbCr_DC);

    huff_table *htable_CbCr_AC = huffman_table_build(htables_nb_symb_per_lengths[AC][Cb], htables_symbols[AC][Cb], htables_nb_symbols[AC][Cb]);
    if (htable_CbCr_AC == NULL) {
        jpeg_destroy(jpg);
        exit(EXIT_FAILURE);
    }
    jpeg_set_huffman_table(jpg, AC, Cb, htable_CbCr_AC);

    printf("\nEcriture des sampling-factors\n");
    uint8_t count = 0;
    for (size_t cc = Y; cc < NB_COLOR_COMPONENTS; cc++) {
        for (size_t dir = H; dir < NB_DIRECTIONS; dir++) {
            jpeg_set_sampling_factor(jpg, cc, dir, count);
            uint8_t sampling_factor = jpeg_get_sampling_factor(jpg, cc, dir);
            printf("Sampling-Factor: %u\n", sampling_factor);
            count++;
        }
    }

    printf("\nEcriture du header\n");
    jpeg_write_header(jpg);

    printf("\nEcriture du footer\n");
    jpeg_write_footer(jpg);

    printf("\nDestruction de la structure jpeg\n");
    jpeg_destroy(jpg);
}
