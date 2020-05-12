#ifndef CONV_H
#define CONV_H

#include "../include/ppm.h"

/* Définition d'un triplet de couleurs YCbCr */
typedef struct triplet_ycbcr triplet_ycbcr;

struct triplet_ycbcr {
	uint8_t triplet[3];
};

/*
 * type: triplet_rgb*
 * rtype: triplet_ycbcr*
 * Convertit un pixel RGB en YCbCr
 */
triplet_ycbcr* conversion(triplet_rgb* rgb);

#endif
