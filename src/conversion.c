#include "../include/conversion.h"

#include "../include/ppm.h"

triplet_ycbcr* conversion(triplet_rgb* rgb) {
    triplet_ycbcr* ycbcr = (triplet_ycbcr*)malloc(sizeof(triplet_ycbcr));

    ycbcr->triplet[0] = 0.299 * rgb->triplet[R] + 0.587 * rgb->triplet[G] + 0.114 * rgb->triplet[B];
    ycbcr->triplet[1] = -0.1687 * rgb->triplet[R] - 0.3313 * rgb->triplet[G] + 0.5000 * rgb->triplet[B] + 128.0;
    ycbcr->triplet[2] = 0.5000 * rgb->triplet[R] - 0.4187 * rgb->triplet[G] - 0.0813 * rgb->triplet[B] + 128.0;

    return ycbcr;
}
