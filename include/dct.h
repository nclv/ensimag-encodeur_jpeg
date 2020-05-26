#ifndef _DCT_H
#define _DCT_H

#include <stdint.h>

extern void dct(int16_t **input, int16_t **output);

extern void offset(int16_t **input);

#endif
