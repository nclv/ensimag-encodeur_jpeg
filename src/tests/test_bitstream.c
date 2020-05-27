#include "bitstream.h"

int main(void) {
    bitstream* stream = bitstream_create("test.jpg");

    /*
        Ecrit dans le flux les 6 bits représentant la valeur 42
        (42 c'est 0x2A, ou 101010 en binaire)
    */
    bitstream_write_bits(stream, 42, 6, false);
    bitstream_display(stream);
    bitstream_write_bits(stream, 42, 9, false);
    bitstream_display(stream);
    // bitstream_flush(stream);

    bitstream_write_bits(stream, 42, 6, false);
    bitstream_display(stream);

    bitstream_write_bits(stream, 0xffd9, 16, true);
    bitstream_display(stream);

    bitstream_write_bits(stream, 0xff, 8, false);
    bitstream_display(stream);

    bitstream_destroy(stream);
}
