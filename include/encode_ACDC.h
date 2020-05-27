#ifndef _ENCODE_ACDC_H
#define _ENCODE_ACDC_H

#include <stdint.h>

#include "bitstream.h"
#include "huffman.h"

/* type: bitstream*, const int16_t[8][8], huff_table*, huff_table*, int16_t
 * rtype: void
 * Encode chaque coefficient de la data unit 8x8 DC et AC avec les codes RLE
 * et les tables de huffman et écrit les symboles dans le bitstream
 */
extern void ecrire_coeffs(bitstream *stream, const int16_t data_unit[8][8], huff_table *dc_table, huff_table *ac_table, int16_t difference_DC);

#endif
