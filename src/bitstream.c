#include "bitstream.h"


#define DEFAULT_BUFFER_SIZE 512

bitstream *bitstream_create(const char *filename) {
    
    bitstream *stream = malloc(sizeof *stream);
    if (stream == NULL) return NULL;

    stream->buffer_size = DEFAULT_BUFFER_SIZE;
    stream->buffer = malloc(DEFAULT_BUFFER_SIZE);
    if (stream->buffer = NULL) {
        free(stream);
        return NULL;
    }

    stream->filename = filename;
    stream->last_written_bit_offset = 0;
    stream->current_bit_offset = 0;
    stream->current_buffer_offset = 0;

    return stream;
}



void bitstream_destroy(struct bitstream *stream) {
    free(stream->buffer);
    free(stream);
}