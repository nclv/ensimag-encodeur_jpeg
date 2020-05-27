#include "bitstream.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

bitstream *bitstream_create(const char *filename) {
    bitstream *stream = malloc(sizeof *stream);
    if (stream == NULL) return NULL;

    stream->bytes_buffer_size = DEFAULT_BUFFER_SIZE;
    /* On choisit d'allouer sur le stack, moins rapide mais plus efficace en mémoire */
    // stream->bytes_buffer = malloc((sizeof(unsigned char *) + 1) * DEFAULT_BUFFER_SIZE);
    if (stream->bytes_buffer == NULL) {
        free(stream);
        return NULL;
    }
    stream->last_written_byte_offset = 0;

    stream->bits_buffer_size = 8;
    stream->bits_buffer = 0;
    stream->last_written_bit_offset = 0;

    stream->file = fopen(filename, "ab");
    if (stream->file == NULL) {
        perror("Error opening file: ");
        return NULL;
    }

    return stream;
}

void bitstream_display(bitstream *stream) {
    assert(stream != NULL);
    printf("\nDébut de l'affichage\n");

    // printf("Filename: %s\n", stream->filename);

    printf("Bits buffer size: %ld\n", stream->bits_buffer_size);
    printf("Nombre de bits dans le buffer: %ld\n", stream->last_written_bit_offset);
    // printf("Bits buffer: %02x\n", stream->bits_buffer);

    printf("Bytes buffer size: %ld\n", stream->bytes_buffer_size);
    printf("Nombre de bytes dans le buffer: %ld\n", stream->last_written_byte_offset);
    printf("Bytes buffer: \n");
    for (uint32_t i = 0; i < stream->last_written_byte_offset; i++) {
        printf("%02x ", stream->bytes_buffer[i]);
    }
    printf("\n");
}

void bitstream_flush(bitstream *stream) {
    fwrite(stream->bytes_buffer, sizeof(stream->bytes_buffer[0]), stream->last_written_byte_offset, stream->file);
    stream->last_written_byte_offset = 0;
}

/* Ecriture dans bytes_buffer lorsque bits_buffer contient 8 bits*/
static void bitstream_write_byte(bitstream *stream) {
    // printf("\nEcriture d'un byte\n");
    /* Dépassement de la capacité du buffer, pas d'erreur visible sauf sur valgrind */
    if (stream->last_written_byte_offset >= stream->bytes_buffer_size) {
        printf("Dépassement de la capacité du buffer\n");
        // bitstream_display(stream);
        bitstream_flush(stream);
    }

    stream->bytes_buffer[stream->last_written_byte_offset] = stream->bits_buffer;
    stream->last_written_byte_offset++;

    stream->last_written_bit_offset = 0;
}

/* Ecriture de bit dans bits_buffer */
static void bitstream_write_bit(bitstream *stream, unsigned char bit) {
    stream->bits_buffer = (unsigned char)(stream->bits_buffer << 1);
    stream->bits_buffer = (unsigned char)(stream->bits_buffer | bit);
    stream->last_written_bit_offset++;
}

void bitstream_write_bits(bitstream *stream, uint32_t value, uint8_t nb_bits, bool is_marker) {
    assert(nb_bits < 32);  // au plus 32 bits

    printf("Marqueur: %i, writing %i over %i bits \n", is_marker, value, nb_bits);
    /* Un marqueur de section JPEG est toujours aligné dans le flux sur un multiple d'un octet */
    if (is_marker) {
        printf("Ecriture d'un marqueur (EOI)\n");
        bitstream_flush(stream);
        // On complète bits_buffer et on écrit le byte
        size_t padding = (stream->bits_buffer_size - stream->last_written_bit_offset);
        stream->bits_buffer = (unsigned char)(stream->bits_buffer << padding);
        stream->last_written_bit_offset = stream->last_written_bit_offset + padding;
        bitstream_write_byte(stream);
    }

    uint32_t mask = 1U << (nb_bits - 1);
    for (size_t i = 0; i < nb_bits; i++) {
        unsigned char bit = (value & mask) ? 1 : 0;
        // printf("%d ", bit);
        bitstream_write_bit(stream, bit);

        // 8 bits dans bits_buffer, on écrit un byte dans bytes_buffer
        if (stream->last_written_bit_offset == stream->bits_buffer_size) {
            bitstream_write_byte(stream);
            if (stream->bits_buffer == 0xff && !(is_marker)) {
                printf("Ecriture du stuffing byte 0x00 dans bytes_buffer\n");
                stream->bits_buffer = 0;
                bitstream_write_byte(stream);
            }
        }

        value <<= 1;
    }
}

void bitstream_destroy(bitstream *stream) {
    if (stream != NULL) {
        // free(stream->bytes_buffer);
        fclose(stream->file);
        free(stream);
    }
}
