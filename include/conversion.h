#ifndef _CONV_H
#define _CONV_H

#include <stdint.h>

#include "../include/ppm.h"

/* Définition et déclaration d'un triplet de couleurs YCbCr */
/* Y is defined to have a range of 16 to 235; Cb and Cr are defined to have a range of 16 to 240*/
typedef struct triplet_ycbcr {
    uint8_t triplet[3];
} triplet_ycbcr;

/*
 * type: triplet_rgb*
 * rtype: triplet_ycbcr*
 * Convertit un pixel RGB en YCbCr
 */
triplet_ycbcr* conversion(triplet_rgb* rgb);

#endif
