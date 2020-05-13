#include "conversion.h"

#include <stdint.h>

#include "../include/ppm.h"

void conversion(pixel* p) {
    p->triplet_ycbcr[0] = (uint8_t) (0.299 * p->triplet_rgb[0] + 0.587 * p->triplet_rgb[1] + 0.114 * p->triplet_rgb[2]);
    p->triplet_ycbcr[1] = (uint8_t) (-0.1687 * p->triplet_rgb[0] - 0.3313 * p->triplet_rgb[1] + 0.5000 * p->triplet_rgb[2] + 128);
    p->triplet_ycbcr[2] = (uint8_t) (0.5000 * p->triplet_rgb[0] - 0.4187 * p->triplet_rgb[1] - 0.0813 * p->triplet_rgb[2] + 128);
}
