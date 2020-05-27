#include "jpeg_writer.h"

#include <stdio.h>
#include <stdlib.h>

jpeg *jpeg_create(void) {
    jpeg *jpg = malloc(sizeof *jpg);
    if (jpg == NULL) return NULL;

    jpg->ht_Y_DC = NULL;
    jpg->ht_Y_AC = NULL;
    jpg->ht_CbCr_DC = NULL;
    jpg->ht_CbCr_AC = NULL;

    jpg->jpeg_filename = NULL;
    jpg->input_filename = NULL;
    jpg->stream = NULL;

    jpg->image_height = 256;
    jpg->image_width = 512;
    jpg->nb_components = 0;

    jpg->Y_quantization_table = NULL;
    jpg->CbCr_quantization_table = NULL;

    return jpg;
}

void jpeg_destroy(jpeg *jpg) {
    if (jpg->ht_Y_DC != NULL) huffman_table_destroy(jpg->ht_Y_DC);
    if (jpg->ht_Y_AC != NULL) huffman_table_destroy(jpg->ht_Y_AC);
    if (jpg->ht_CbCr_DC != NULL) huffman_table_destroy(jpg->ht_CbCr_DC);
    if (jpg->ht_CbCr_AC != NULL) huffman_table_destroy(jpg->ht_CbCr_AC);
    bitstream_destroy(jpg->stream);
    free(jpg);
}

bitstream *jpeg_get_bitstream(jpeg *jpg) {
    return jpg->stream;
}

/* type : FILE*
 * type : huff_table*
 * type : const char*
 * rtype : void
 * Section de définition de la (ou les) table(s) de Huffman.
 */
static void jpeg_write_dht_section(FILE *file, huff_table *ht, const char *HT_TYPE) {
    const unsigned char DHT[2] = {0xff, 0xc4};
    fwrite(DHT, sizeof DHT, 1, file);

    // 32 et pas 16 pour éviter l'erreur de conversion "conversion from ‘int’ to ‘unsigned char’ may change value"
    uint32_t dht_len = (uint32_t)(19 + ht->nb_symbols);  // la longueur de la section dépend du codage
    /* On doit inverser les octets pour l'écriture */
    const unsigned char LEN[2] = {(dht_len >> 8) & 0xff,
                                  dht_len & 0xff};
    fwrite(LEN, sizeof LEN, 1, file);

    fwrite(HT_TYPE, 1, 1, file);

    uint8_t *nb_symb_per_lengths = huffman_table_get_length_vector(ht);
    fwrite(nb_symb_per_lengths, sizeof(nb_symb_per_lengths[0]), 16, file);
    // for (size_t i = 0; i < 16; i++) {
    //     fwrite(&nb_symb_per_lengths[i], 1, 1, file);
    // }

    uint8_t *symbols = huffman_table_get_symbols(ht);
    fwrite(symbols, sizeof(symbols[0]), ht->nb_symbols, file);
    // for (size_t i = 0; i < ht->nb_symbols; i++) {
    //     fwrite(&symbols[i], 1, 1, file);
    // }
}

/* type : jpeg*
 * rtype : void
 * Ecriture de l'en-tête jpeg pour les images en niveau de gris.
 */
static void jpeg_write_header_grayscale(jpeg *jpg) {
    FILE *file = fopen(jpg->jpeg_filename, "wb");

    const unsigned char SOI[2] = {0xff, 0xd8};
    const unsigned char APP0[2] = {0xff, 0xe0};
    const unsigned char DQT[2] = {0xff, 0xdb};
    const unsigned char SOF0[2] = {0xff, 0xc0};
    const unsigned char SOS[2] = {0xff, 0xda};

    /* On doit inverser les octets pour l'écriture */
    const unsigned char HEIGTH[2] = {(jpg->image_height >> 8) & 0xff,
                                     jpg->image_height & 0xff};
    const unsigned char WIDTH[2] = {(jpg->image_width >> 8) & 0xff,
                                    jpg->image_width & 0xff};

    /* 8-bit integer type donc pas besoin de & 0xff */
    uint8_t y_sampling_factor = (uint8_t)((jpg->sampling_factors[Y][H] << 4) | jpg->sampling_factors[Y][V]);

    fwrite(SOI, sizeof SOI, 1, file);

    fwrite(APP0, sizeof APP0, 1, file);
    fwrite("\x00\x10", 2, 1, file);                      // longueur de la section sur 2 octets
    fwrite("\x4a\x46\x49\x46\x00", 5, 1, file);          // JFIF\0
    fwrite("\x01\x01", 2, 1, file);                      // version JFIF 1.1
    fwrite("\x00\x00\x00\x00\x00\x00\x00", 7, 1, file);  // Données spécifiques au JFIF, non traitées

    /* Pas de section COM */

    /* Y quantization table */
    fwrite(DQT, sizeof DQT, 1, file);
    fwrite("\x00\x43", 2, 1, file);  // longueur de la section sur 2 octets
    fwrite("\x00", 1, 1, file);      // precision and quantization table index
    fwrite(jpg->Y_quantization_table, sizeof(jpg->Y_quantization_table[0]), 64, file);
    // for (size_t i = 0; i < 64; i++) {
    //     /* Les tables de quantification sont déja stockées au format zig-zag */
    //     fwrite(&jpg->Y_quantization_table[i], 1, 1, file);
    // }
    // fwrite(jpg->Y_quantization_table, sizeof jpg->Y_quantization_table, 8, file);
    // fwrite(jpg->Y_quantization_table, sizeof(uint8_t), 64, file);

    fwrite(SOF0, sizeof SOF0, 1, file);
    /* Grayscale: 11, RGB: 17*/
    fwrite("\x00\x0b", 2, 1, file);  // longueur de la section sur 2 octets

    fwrite("\x08", 1, 1, file);  // sample precision

    fwrite(HEIGTH, sizeof HEIGTH, 1, file);
    fwrite(WIDTH, sizeof WIDTH, 1, file);

    /* Grayscale: 1, RGB: 3*/
    fwrite("\x01", 1, 1, file);  // nombre de composantes

    fwrite("\x01", 1, 1, file);  // id
    fwrite(&y_sampling_factor, 1, 1, file);
    fwrite("\x00", 1, 1, file);  // quantization table index

    /* Ecriture des tables de Huffman */
    jpeg_write_dht_section(file, jpg->ht_Y_DC, "\x00");
    jpeg_write_dht_section(file, jpg->ht_Y_AC, "\x10");

    /* Une seule section SOS car "baseline sequential" */
    fwrite(SOS, sizeof SOS, 1, file);
    /* Grayscale: 8, RGB: 12 */
    fwrite("\x00\x08", 2, 1, file);  // longueur de la section sur 2 octets
    /* Grayscale: 1, RGB: 3*/
    fwrite("\x01", 1, 1, file);  // nombre de composantes

    /* DC and AC Huffman table indexes */
    fwrite("\x01", 1, 1, file);  // id Y
    fwrite("\x00", 1, 1, file);  // huff_dc / huff_ac

    fwrite("\x00", 1, 1, file);  // Premier indice de la sélection spectrale
    fwrite("\x63", 1, 1, file);  // Dernier indice de la sélection spectrale
    fwrite("\x00", 1, 1, file);  // Approximation successive

    /* Fin du header, écriture des coefficients */

    fclose(file);
}

/* type : jpeg*
 * rtype : void
 * Ecriture de l'en-tête jpeg pour les images en couleur.
 */
static void jpeg_write_header_RGB(jpeg *jpg) {
    FILE *file = fopen(jpg->jpeg_filename, "wb");

    const unsigned char SOI[2] = {0xff, 0xd8};
    const unsigned char APP0[2] = {0xff, 0xe0};
    const unsigned char DQT[2] = {0xff, 0xdb};
    const unsigned char SOF0[2] = {0xff, 0xc0};
    const unsigned char SOS[2] = {0xff, 0xda};

    /* On doit inverser les octets pour l'écriture */
    /* The masking is to avoid implicit sign extension. */
    const unsigned char HEIGTH[2] = {(jpg->image_height >> 8) & 0xff,
                                     jpg->image_height & 0xff};
    const unsigned char WIDTH[2] = {(jpg->image_width >> 8) & 0xff,
                                    jpg->image_width & 0xff};

    /* 8-bit integer type donc pas besoin de & 0xff */
    uint8_t y_sampling_factor = (uint8_t)((jpg->sampling_factors[Y][H] << 4) | jpg->sampling_factors[Y][V]);
    uint8_t cb_sampling_factor = (uint8_t)((jpg->sampling_factors[Cb][H] << 4) | jpg->sampling_factors[Cb][V]);
    uint8_t cr_sampling_factor = (uint8_t)((jpg->sampling_factors[Cr][H] << 4) | jpg->sampling_factors[Cr][V]);


    fwrite(SOI, sizeof SOI, 1, file);

    fwrite(APP0, sizeof APP0, 1, file);
    fwrite("\x00\x10", 2, 1, file);                      // longueur de la section sur 2 octets
    fwrite("\x4a\x46\x49\x46\x00", 5, 1, file);          // JFIF\0
    fwrite("\x01\x01", 2, 1, file);                      // version JFIF 1.1
    fwrite("\x00\x00\x00\x00\x00\x00\x00", 7, 1, file);  // Données spécifiques au JFIF, non traitées

    /* Pas de section COM */

    /* Y quantization table */
    fwrite(DQT, sizeof DQT, 1, file);
    fwrite("\x00\x43", 2, 1, file);  // longueur de la section sur 2 octets
    fwrite("\x00", 1, 1, file);      // precision and quantization table index
    fwrite(jpg->Y_quantization_table, sizeof(jpg->Y_quantization_table[0]), 64, file);
    // for (size_t i = 0; i < 64; i++) {
    //     /* Les tables de quantification sont déja stockées au format zig-zag */
    //     fwrite(&jpg->Y_quantization_table[i], 1, 1, file);
    // }
    // fwrite(jpg->Y_quantization_table, sizeof jpg->Y_quantization_table, 8, file);
    // fwrite(jpg->Y_quantization_table, sizeof(uint8_t), 64, file);

    /* Cb / Cr quantization table */
    fwrite(DQT, sizeof DQT, 1, file);
    fwrite("\x00\x43", 2, 1, file);  // longueur de la section sur 2 octets
    fwrite("\x01", 1, 1, file);      // precision and quantization table index
    fwrite(jpg->CbCr_quantization_table, sizeof(jpg->CbCr_quantization_table[0]), 64, file);
    // for (size_t i = 0; i < 64; i++) {
    //     /* Les tables de quantification sont déja stockées au format zig-zag */
    //     fwrite(&jpg->CbCr_quantization_table[i], 1, 1, file);
    // }

    fwrite(SOF0, sizeof SOF0, 1, file);
    /* Grayscale: 11, RGB: 17*/
    fwrite("\x00\x11", 2, 1, file);  // longueur de la section sur 2 octets

    fwrite("\x08", 1, 1, file);  // sample precision

    fwrite(HEIGTH, sizeof HEIGTH, 1, file);
    fwrite(WIDTH, sizeof WIDTH, 1, file);

    /* Grayscale: 1, RGB: 3*/
    fwrite("\x03", 1, 1, file);  // nombre de composantes

    fwrite("\x01", 1, 1, file);  // id
    fwrite(&y_sampling_factor, 1, 1, file);
    fwrite("\x00", 1, 1, file);  // quantization table index

    fwrite("\x01", 1, 1, file);  // id
    fwrite(&cb_sampling_factor, 1, 1, file);
    fwrite("\x01", 1, 1, file);

    fwrite("\x01", 1, 1, file);  // id
    fwrite(&cr_sampling_factor, 1, 1, file);
    fwrite("\x01", 1, 1, file);

    /* Ecriture des tables de Huffman */
    jpeg_write_dht_section(file, jpg->ht_Y_DC, "\x00");
    jpeg_write_dht_section(file, jpg->ht_Y_AC, "\x10");
    jpeg_write_dht_section(file, jpg->ht_CbCr_DC, "\x01");
    jpeg_write_dht_section(file, jpg->ht_CbCr_AC, "\x11");

    /* Une seule section SOS car "baseline sequential" */
    fwrite(SOS, sizeof SOS, 1, file);
    /* Grayscale: 8, RGB: 12 */
    fwrite("\x00\x0c", 2, 1, file);  // longueur de la section sur 2 octets
    /* Grayscale: 1, RGB: 3*/
    fwrite("\x03", 1, 1, file);  // nombre de composantes

    /* DC and AC Huffman table indexes */
    fwrite("\x01", 1, 1, file);  // id Y
    fwrite("\x00", 1, 1, file);  // huff_dc / huff_ac
    fwrite("\x02", 1, 1, file);  // id Cb
    fwrite("\x11", 1, 1, file);
    fwrite("\x03", 1, 1, file);  // id Cr
    fwrite("\x11", 1, 1, file);

    fwrite("\x00", 1, 1, file);  // Premier indice de la sélection spectrale
    fwrite("\x63", 1, 1, file);  // Dernier indice de la sélection spectrale
    fwrite("\x00", 1, 1, file);  // Approximation successive

    /* Fin du header, on passe à l'écriture des coefficients */

    fclose(file);
}

void jpeg_write_header(jpeg *jpg) {
    jpg->stream = bitstream_create(jpg->jpeg_filename);
    (jpg->nb_components == 1) ? jpeg_write_header_grayscale(jpg) : jpeg_write_header_RGB(jpg);
}

void jpeg_write_footer(jpeg *jpg) {
    bitstream_write_bits(jpg->stream, 0xffd9, 16, true);
    bitstream_flush(jpg->stream);
}

/* Setters and getters */

void jpeg_set_ppm_filename(jpeg *jpg, const char *input_filename) {
    jpg->input_filename = input_filename;
}

char *jpeg_get_ppm_filename(jpeg *jpg) {
    return (char *)jpg->input_filename;
}

void jpeg_set_jpeg_filename(jpeg *jpg, const char *jpeg_filename) {
    jpg->jpeg_filename = jpeg_filename;
}

char *jpeg_get_jpeg_filename(jpeg *jpg) {
    return (char *)jpg->jpeg_filename;
}

void jpeg_set_image_height(jpeg *jpg, uint32_t image_height) {
    jpg->image_height = image_height;
}

uint32_t jpeg_get_image_height(jpeg *jpg) {
    return jpg->image_height;
}

void jpeg_set_image_width(jpeg *jpg, uint32_t image_width) {
    jpg->image_width = image_width;
}

uint32_t jpeg_get_image_width(jpeg *jpg) {
    return jpg->image_width;
}

void jpeg_set_nb_components(jpeg *jpg, uint8_t nb_components) {
    jpg->nb_components = nb_components;
}

uint8_t jpeg_get_nb_components(jpeg *jpg) {
    return jpg->nb_components;
}

void jpeg_set_quantization_table(jpeg *jpg, enum color_component cc, uint8_t *qtable) {
    switch (cc) {
        case Y:
            jpg->Y_quantization_table = qtable;
            break;
        case Cb:
        case Cr:
            jpg->CbCr_quantization_table = qtable;
            break;
        default:
            return;
    }
}

uint8_t *jpeg_get_quantization_table(jpeg *jpg, enum color_component cc) {
    switch (cc) {
        case Y:
            return jpg->Y_quantization_table;
        case Cb:
        case Cr:
            return jpg->CbCr_quantization_table;
        default:
            return NULL;
    }
}

void jpeg_set_huffman_table(jpeg *jpg, enum sample_type acdc, enum color_component cc, huff_table *htable) {
    switch (cc) {
        case Y:
            switch (acdc) {
                case DC:
                    jpg->ht_Y_DC = htable;
                    break;
                case AC:
                    jpg->ht_Y_AC = htable;
                    break;
                default:
                    return;
            }
            break;
        case Cb:
        case Cr:
            switch (acdc) {
                case DC:
                    jpg->ht_CbCr_DC = htable;
                    break;
                case AC:
                    jpg->ht_CbCr_AC = htable;
                    break;
                default:
                    return;
            }
            break;
        default:
            return;
    }
}

huff_table *jpeg_get_huffman_table(jpeg *jpg, enum sample_type acdc, enum color_component cc) {
    switch (cc) {
        case Y:
            switch (acdc) {
                case DC:
                    return jpg->ht_Y_DC;
                case AC:
                    return jpg->ht_Y_AC;
                default:
                    return NULL;
            }
        case Cb:
        case Cr:
            switch (acdc) {
                case DC:
                    return jpg->ht_CbCr_DC;
                case AC:
                    return jpg->ht_CbCr_AC;
                default:
                    return NULL;
            }
        default:
            return NULL;
    }
}

void jpeg_set_sampling_factor(jpeg *jpg, enum color_component cc, enum direction dir, uint8_t sampling_factor) {
    jpg->sampling_factors[cc][dir] = sampling_factor;
}

uint8_t jpeg_get_sampling_factor(jpeg *jpg, enum color_component cc, enum direction dir) {
    return jpg->sampling_factors[cc][dir];
}
