#include <stdio.h>
#include <stdlib.h>

#include "htables.h"
#include "jpeg_writer.h"
#include "qtables.h"

int main(void) {
    jpeg *jpg = jpeg_create();

    jpeg_set_jpeg_filename(jpg, "output.jpeg");
    jpeg_set_quantization_table(jpg, Y, quantification_table_Y);
    jpeg_set_quantization_table(jpg, Cb, quantification_table_CbCr);

    printf("Y_qtables\n");
    uint8_t *Y_qtables = jpeg_get_quantization_table(jpg, Y);
    for (size_t i = 0; i < 64; i += 8) {
        for (size_t j = 0; j < 8; j++) {
            printf("%x, ", Y_qtables[i + j]);
        }
        printf("\n");
    }

    printf("CbCr_qtables\n");
    uint8_t *CbCr_qtables = jpeg_get_quantization_table(jpg, Cr);
    for (size_t i = 0; i < 64; i += 8) {
        for (size_t j = 0; j < 8; j++) {
            printf("%x, ", CbCr_qtables[i + j]);
        }
        printf("\n");
    }

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

    jpeg_write_header(jpg);

    jpeg_destroy(jpg);
}