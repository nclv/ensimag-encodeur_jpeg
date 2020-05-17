#include "jpeg_writer.h"

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

    jpg->image_height = NULL;
    jpg->image_width = NULL;
    jpg->nb_components = NULL;

    jpg->sampling_factor = NULL;
    jpg->quantization_tables = NULL;
}

void jpeg_destroy(jpeg *jpg) {
    free(jpg->huffman_table);
    free(jpg);
}

bitstream *jpeg_get_bitstream(jpeg *jpg) {
    return bitstream_create(jpg->jpeg_filename);
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