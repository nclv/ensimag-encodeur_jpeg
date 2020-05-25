#include "bitstream.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define DEFAULT_BUFFER_SIZE 1  // valeur à modifier, 1 pour les tests

bitstream *bitstream_create(const char *filename) {
    bitstream *stream = malloc(sizeof *stream);
    if (stream == NULL) return NULL;

    stream->buffer_size = DEFAULT_BUFFER_SIZE;
    stream->buffer = malloc((sizeof(unsigned char *) + 1) * DEFAULT_BUFFER_SIZE);
    /* printf("%ld", sizeof stream->buffer); */
    /* Renvoie 8 * DEFAULT_BUFFER_SIZE */
    if (stream->buffer == NULL) {
        free(stream);
        return NULL;
    }

    stream->filename = filename;
    stream->last_written_bit_offset = 0;

    return stream;
}

void bitstream_display(bitstream *stream) {
    assert(stream != NULL);

    printf("Filename: %s\n", stream->filename);
    printf("Buffer size: %ld\n", stream->buffer_size * 8);
    printf("Nombre de bits dans le buffer: %ld\n", stream->last_written_bit_offset);

    /* Affichage du buffer */
    for (uint32_t i = 0; i < stream->last_written_bit_offset; i++) {
        printf("%d", stream->buffer[i]);
    }
    printf("\n");
}

void bitstream_flush(bitstream *stream) {
    size_t length = stream->last_written_bit_offset;
    for (size_t i = 0; i < length; i++) {
        printf("Ecriture du bit %d dans le fichier jpeg\n", stream->buffer[i]);
        stream->buffer[i] = 0;
        stream->last_written_bit_offset--;
    }
}

void bitstream_write_bits(bitstream *stream, uint32_t value, uint8_t nb_bits, bool is_marker) {
    assert(0 < nb_bits && nb_bits <= 32);

    printf("Marqueur: %i\n", is_marker);

    /* Dépassement de la capacité du buffer, pas d'erreur visible sauf sur valgrind */
    /* Un marqueur de section JPEG est toujours aligné dans le flux sur un multiple d'un octet*/
    if (DEFAULT_BUFFER_SIZE * 8 - stream->last_written_bit_offset < nb_bits || is_marker) {
        printf("Dépassement de la capacité du buffer ou écriture d'un marqueur\n");
        bitstream_display(stream);
        bitstream_flush(stream);
    }

    uint32_t mask = 1U << (nb_bits - 1);

    size_t current_bit_offset = stream->last_written_bit_offset;
    for (size_t i = 0; i < nb_bits; i++) {
        stream->buffer[current_bit_offset + i] = (value & mask) ? 1 : 0;
        value <<= 1;
        stream->last_written_bit_offset += 1;
    }
}

void bitstream_destroy(bitstream *stream) {
    if (stream != NULL) {
        free(stream->buffer);
        free(stream);
    }
}
