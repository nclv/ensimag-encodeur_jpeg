#include "bitstream.h"

int main(void) {
    bitstream* stream = bitstream_create("test.jpg");

    /* écrit la valeur 42 stockée sur 5 bits dans le flux */
    bitstream_write_nbits(stream, 5, 42, false);

    bitstream_destroy(stream);
}
