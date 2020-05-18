#ifndef _DCT_H
#define _DCT_H

#include <stdint.h>

extern void dct(int8_t input[8][8], int16_t output[8][8]);

extern void offset(int8_t input[8][8]);

#endif
