#include "bitstream.h"


#define DEFAULT_BUFFER_SIZE 512

bitstream *bitstream_create(const char *filename) {
    
    bitstream *bs = malloc(sizeof *bs);
    if (bs == NULL) return NULL;

    bs->buffer_size = DEFAULT_BUFFER_SIZE;
    bs->buffer = malloc(DEFAULT_BUFFER_SIZE);
    if (bs->buffer = NULL) {
        free(bs);
        return NULL;
    }

    bs->filename = filename;
    bs->last_written_bit_offset = 0;
    bs->current_bit_offset = 0;
    bs->current_buffer_offset = 0;

    return bs;
}
