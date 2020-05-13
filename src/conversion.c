#include "../include/conversion.h"

#include "../include/ppm.h"

void conversion(pixel* pixel) {

    float y = 0.299 * pixel->triplet[0] + 0.587 * pixel->triplet[1] + 0.114 * pixel->triplet[2];
    float cb = -0.1687 * pixel->triplet[0] - 0.3313 * pixel->triplet[1] + 0.5000 * pixel->triplet[2] + 128;
    float cr = 0.5000 * pixel->triplet[0] - 0.4187 * pixel->triplet[1] - 0.0813 * pixel->triplet[2] + 128;
    pixel->triplet[0] = y;
    pixel->triplet[1] = cb;
    pixel->triplet[2] =cr;
}
