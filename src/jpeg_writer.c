#include "jpeg_writer.h"

#include <stdio.h>
#include <stdlib.h>

#include "qtables.h"
#include "zigzag.h"

jpeg *jpeg_create(void) {
    jpeg *jpg = malloc(sizeof *jpg);
    if (jpg == NULL) return NULL;

    jpg->huffman_table = malloc(sizeof jpg->huffman_table);
    if (jpg->huffman_table == NULL) {
        free(jpg);
        return NULL;
    }

    jpg->jpeg_filename = NULL;
    jpg->ppm_filename = NULL;

    jpg->image_height = 256;
    jpg->image_width = 512;
    jpg->nb_components = 0;

    jpg->y_sampling_factor = 0;
    jpg->cb_sampling_factor = 0;
    jpg->cr_sampling_factor = 0;

    jpg->Y_quantization_table = NULL;
    jpg->CbCr_quantization_table = NULL;

    return jpg;
}

void jpeg_destroy(jpeg *jpg) {
    free(jpg->huffman_table);
    free(jpg);
}

bitstream *jpeg_get_bitstream(jpeg *jpg) {
    return bitstream_create(jpg->jpeg_filename);
}

void jpeg_write_header(jpeg *jpg) {
    FILE *file = fopen(jpg->jpeg_filename, "wb");

    const unsigned char SOI[2] = {0xff, 0xd8};
    const unsigned char APP0[2] = {0xff, 0xe0};
    const unsigned char DQT[2] = {0xff, 0xdb};
    const unsigned char SOF0[2] = {0xff, 0xc0};
    const unsigned char DHT[2] = {0xff, 0xc4};
    const unsigned char SOS[2] = {0xff, 0xda};

    /* On doit inverser les octets pour l'écriture */
    const unsigned char HEIGTH[2] = {(jpg->image_height >> 8) & 0xff,
                                     jpg->image_height & 0xff};
    const unsigned char WIDTH[2] = {(jpg->image_width >> 8) & 0xff,
                                    jpg->image_width & 0xff};

    fwrite(SOI, sizeof SOI, 1, file);

    fwrite(APP0, sizeof APP0, 1, file);
    fwrite("\x00\x16", 2, 1, file);                      // longueur de la section sur 2 octets
    fwrite("\x4a\x46\x49\x46\x00", 5, 1, file);          // JFIF\0
    fwrite("\x01\x01", 2, 1, file);                      // version JFIF 1.1
    fwrite("\x00\x00\x00\x00\x00\x00\x00", 7, 1, file);  // Données spécifiques au JFIF, non traitées

    /* Pas de section COM */

    /* Y quantization table */
    fwrite(DQT, sizeof DQT, 1, file);
    fwrite("\x00\x67", 2, 1, file);  // longueur de la section sur 2 octets
    fwrite("\x00", 1, 1, file);      // precision and quantization table index
    for (size_t i = 0; i < 64; i++) {
        fwrite(&jpg->Y_quantization_table[indices_zigzag[i]], 1, 1, file);
    }

    /* Cb / Cr quantization table */
    fwrite(DQT, sizeof DQT, 1, file);
    fwrite("\x00\x67", 2, 1, file);  // longueur de la section sur 2 octets
    fwrite("\x01", 1, 1, file);      // precision and quantization table index
    for (size_t i = 0; i < 64; i++) {
        fwrite(&jpg->CbCr_quantization_table[indices_zigzag[i]], 1, 1, file);
    }

    fwrite(SOF0, sizeof SOF0, 1, file);
    /* Grayscale: 11, RGB: 17*/
    fwrite("\x00\x11", 2, 1, file);  // longueur de la section sur 2 octets
    // fwrite("\x00\x17", 2, 1, file);

    fwrite("\x08", 1, 1, file);  // sample precision

    fwrite(HEIGTH, sizeof HEIGTH, 1, file);
    fwrite(WIDTH, sizeof WIDTH, 1, file);

    /* Grayscale: 1, RGB: 3*/
    fwrite("\x01", 1, 1, file);  // nombre de composantes
    // fwrite("\x03", 1, 1, file);

    fwrite("\x01", 1, 1, file);  // id
    fwrite(&jpg->y_sampling_factor, 1, 1, file);
    fwrite("\x00", 1, 1, file);  // quantization table index

    // fwrite("\x01", 1, 1, file);  // id
    // fwrite(jpg->cb_sampling_factor, 1, 1, file);
    // fwrite("\x01", 1, 1, file);

    // fwrite("\x01", 1, 1, file);  // id
    // fwrite(jpg->cr_sampling_factor, 1, 1, file);
    // fwrite("\x01", 1, 1, file);

    /* Tables de Huffman AC et DC (4 max. pour chaque) */
    fwrite(DHT, sizeof DHT, 1, file);
    // la longueur de la section dépend du codage

    /* Une seule section SOS car "baseline sequential" */
    fwrite(SOS, sizeof SOS, 1, file);
    /* Grayscale: 8, RGB: 18 */
    fwrite("\x00\x08", 2, 1, file);  // longueur de la section sur 2 octets
    // fwrite("\x00\x18", 2, 1, file);
    /* Grayscale: 1, RGB: 3*/
    fwrite("\x01", 1, 1, file);  // nombre de composantes
    // fwrite("\x03", 1, 1, file);

    fwrite("\x01", 1, 1, file);  // id
    /* DC and AC Huffman table indexes */

    fwrite("\x00", 1, 1, file);  // Premier indice de la sélection spectrale
    fwrite("\x63", 1, 1, file);  // Dernier indice de la sélection spectrale
    fwrite("\x00", 1, 1, file);  // Approximation successive

    /* Fin du header, écriture des coefficients */

    fclose(file);
}

void jpeg_write_footer(jpeg *jpg) {
    FILE *file = fopen(jpg->jpeg_filename, "wb");

    const unsigned char EOI[2] = {0xff, 0xd9};

    fwrite(EOI, sizeof EOI, 1, file);   

    fclose(file); 
}

/* Setters and getters */

void jpeg_set_ppm_filename(jpeg *jpg, const char *ppm_filename) {
    jpg->ppm_filename = ppm_filename;
}

char *jpeg_get_ppm_filename(jpeg *jpg) {
    return (char *)jpg->ppm_filename;
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
    switch(cc) {
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
    switch(cc) {
        case Y:
            return jpg->Y_quantization_table;
        case Cb:
        case Cr:
            return jpg->CbCr_quantization_table;
        default:
            return NULL;
    }
}