#include "../include/conversion.h"

#include "../include/ppm.h"

pixel_ycbcr* conversion(pixel_rgb* rgb) {
    pixel_ycbcr* ycbcr = (pixel_ycbcr*)malloc(sizeof(pixel_ycbcr));

    ycbcr->y = 0.299 * rgb->r + 0.587 * rgb->g + 0.114 * rgb->b;
    ycbcr->cb = -0.1687 * rgb->r - 0.3313 * rgb->g + 0.5000 * rgb->b + 128;
    ycbcr->cr = 0.5000 * rgb->r - 0.4187 * rgb->g - 0.0813 * rgb->b + 128;

    return ycbcr;
}
