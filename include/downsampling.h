#ifndef _DOWNSAMPLING_H_
#define _DOWNSAMPLING_H_

#include <stdint.h>

extern void process_Y(int16_t** Y_mcu, uint8_t h1, uint8_t v1, int16_t** data_unit);

extern void process_chroma(int16_t** chroma_mcu, uint8_t h1, uint8_t v1, uint8_t h_chroma, uint8_t v_chroma, int16_t** data_unit);

#endif