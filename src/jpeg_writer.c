#include "jpeg_writer.h"

bitstream *jpeg_get_bitstream(jpeg *jpg) {
    return bitstream_create(jpg->jpeg_filename);
}