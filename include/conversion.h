#ifndef _CONVERSION_H_
#define _CONVERSION_H_

#include <stdint.h>

#include "ppm.h"

/*
 * type: pixel*
 * rtype: void
 * Convertit un pixel RGB en YCbCr
 */
extern pixel* conversion(pixel* pixel);

#endif
