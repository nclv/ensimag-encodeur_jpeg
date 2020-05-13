#include "conversion.h"

#include <stdint.h>

#include "ppm.h"

pixel* conversion(pixel* rgb) {
    pixel* ycbcr = (pixel*)malloc(sizeof(pixel));

    ycbcr->triplet[0] = (uint8_t)(0.299 * rgb->triplet[0] + 0.587 * rgb->triplet[1] + 0.114 * rgb->triplet[2]);
    ycbcr->triplet[1] = (uint8_t)(-0.1687 * rgb->triplet[0] - 0.3313 * rgb->triplet[1] + 0.5000 * rgb->triplet[2] + 128);
    ycbcr->triplet[2] = (uint8_t)(0.5000 * rgb->triplet[0] - 0.4187 * rgb->triplet[1] - 0.0813 * rgb->triplet[2] + 128);

    return ycbcr;
}
