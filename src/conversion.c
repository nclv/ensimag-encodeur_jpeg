#include "../include/conversion.h"

#include "../include/ppm.h"

void conversion(pixel* pixel) {

    pixel->triplet_ycbcr[0] = (uint8_t) (0.299 * pixel->triplet_rgb[0] + 0.587 * pixel->triplet_rgb[1] + 0.114 * pixel->triplet_rgb[2]);
    pixel->triplet_ycbcr[1] = (uint8_t) (-0.1687 * pixel->triplet_rgb[0] - 0.3313 * pixel->triplet_rgb[1] + 0.5000 * pixel->triplet_rgb[2] + 128);
    pixel->triplet_ycbcr[2] = (uint8_t) (0.5000 * pixel->triplet_rgb[0] - 0.4187 * pixel->triplet_rgb[1] - 0.0813 * pixel->triplet_rgb[2] + 128);

}
