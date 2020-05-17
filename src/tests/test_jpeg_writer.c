#include <stdlib.h>

#include "jpeg_writer.h"


int main(void) {
    jpeg *jpg = jpeg_create();

    jpeg_set_jpeg_filename(jpg, "output.jpeg");

    jpeg_write_header(jpg);

    jpeg_destroy(jpg);
}