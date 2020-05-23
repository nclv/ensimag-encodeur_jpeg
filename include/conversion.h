#ifndef _CONVERSION_H_
#define _CONVERSION_H_

#include <stdint.h>

#include "ppm.h"

/*
 * type: MCUs*
 * type: uint8_t**
 * type: uint8_t**
 * type: uint8_t
 * type: uint8_t
 * type: uint8_t
 * rtype: void
 * Convertit un pixel RGB en YCbCr
 * Ne concerne que les images en couleurs
 */
void conversion(MCUs* bloc, size_t i, size_t j, uint8_t r, uint8_t g, uint8_t b);

#endif
