#ifndef _ENCODE_ACDC_H
#define _ENCODE_ACDC_H

#include <stdint.h>

#include "bitstream.h"

extern void ecrire_coeffs(bitstream *stream, int16_t data_unit[8][8], huff_table *dc_table, huff_table *ac_table, int16_t difference_DC);

#endif
