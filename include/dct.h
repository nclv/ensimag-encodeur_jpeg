#ifndef _DCT_H
#define _DCT_H

#include <stdint.h>

extern void dct(int16_t **input, int16_t output[8][8]);

extern void offset(int16_t **input);

#endif
