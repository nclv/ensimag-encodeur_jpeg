#include "bitstream.h"

int main(void) {
    bitstream* stream = bitstream_create("test.jpg");

    /*
        Ecrit dans le flux les 6 bits représentant la valeur 42
        (42 c'est 0x2A, ou 101010 en binaire)
    */
    bitstream_write_bits(stream, 42, 6, false);

    bitstream_destroy(stream);
}
