#include "bitstream.h"

#include <assert.h>
#include <stdio.h>

#define DEFAULT_BUFFER_SIZE 512

bitstream *bitstream_create(const char *filename) {
    bitstream *stream = malloc(sizeof *stream);
    if (stream == NULL) return NULL;

    stream->buffer_size = DEFAULT_BUFFER_SIZE;
    stream->buffer = malloc(DEFAULT_BUFFER_SIZE);
    if (stream->buffer == NULL) {
        free(stream);
        return NULL;
    }

    stream->filename = filename;
    stream->last_written_bit_offset = 0;

    return stream;
}

void bitstream_write_bits(bitstream *stream, uint32_t value, uint8_t nb_bits, bool is_marker) {
    assert(0 < nb_bits && nb_bits <= 32);

    printf("Marqueur: %i\n", is_marker);

    uint32_t mask = 1U << (nb_bits - 1);

    for (uint32_t i = 0; i < nb_bits; i++) {
        stream->buffer[stream->last_written_bit_offset + i] = (value & mask) ? 1 : 0;
        value <<= 1;
    }
    stream->last_written_bit_offset += nb_bits;

    /* Affichage du buffer */ 
    for (uint32_t i = 0; i < stream->last_written_bit_offset; i++) {
        printf("%d", stream->buffer[i]);
    }
    printf("\n");
}

void bitstream_destroy(bitstream *stream) {
    free(stream->buffer);
    free(stream);
}