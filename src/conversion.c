#include <assert.h>
#include <stdint.h>

#include "conversion.h"

void conversion(MCUs* bloc, size_t i, size_t j, uint8_t r, uint8_t g, uint8_t b) {
    bloc->Y[i][j] = (uint8_t)(0.299 * r + 0.587 * g + 0.114 * b);
    bloc->Cb[i][j] = (uint8_t)(-0.1687 * r - 0.3313 * g + 0.5000 * b + 128);
    bloc->Cr[i][j] = (uint8_t)(0.5000 * r - 0.4187 * g - 0.0813 * b + 128);
}
