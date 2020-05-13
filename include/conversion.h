#ifndef _CONV_H
#define _CONV_H

#include "../include/ppm.h"

/* Définition et déclaration d'un triplet de couleurs YCbCr */
typedef struct triplet_ycbcr {
    float triplet[3];
} triplet_ycbcr;

/*
 * type: triplet_rgb*
 * rtype: triplet_ycbcr*
 * Convertit un pixel RGB en YCbCr
 */
triplet_ycbcr* conversion(triplet_rgb* rgb);

#endif
