#ifndef _CONV_H
#define _CONV_H

#include <stdint.h>

#include "../include/ppm.h"

/* Définition et déclaration d'un triplet de couleurs YCbCr */
/* Y is defined to have a range of 16 to 235; Cb and Cr are defined to have a range of 16 to 240*/
typedef struct pixel_ycbcr {
    uint8_t y;
    uint8_t cb;
    uint8_t cr;
} pixel_ycbcr;

/*
 * type: pixel_rgb*
 * rtype: pixel_ycbcr*
 * Convertit un pixel RGB en YCbCr
 */
pixel_ycbcr* conversion(pixel_rgb* rgb);

#endif
