#ifndef _CONVERSION_H_
#define _CONVERSION_H_

#include <stdint.h>

#include "ppm.h"

#define Y 0
#define CB 1
#define CR 2

/*
 * type: pixel*
 * type: uint8_t
 * type: uint8_t
 * type: uint8_t
 * rtype: void
 * Convertit un pixel RGB en YCbCr
 * Ne concerne que les images en couleurs
 */
void conversion(pixel* p, uint8_t r, uint8_t g, uint8_t b);

#endif
