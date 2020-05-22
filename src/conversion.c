#include <assert.h>
#include <stdint.h>

#include "ppm.h"
#include "conversion.h"

void conversion(pixel* p, uint8_t r, uint8_t g, uint8_t b) {
    p->couleurs[0] = (uint8_t)(0.299 * r + 0.587 * g + 0.114 * b);
    p->couleurs[1] = (uint8_t)(-0.1687 * r - 0.3313 * g + 0.5000 * b + 128);
    p->couleurs[2] = (uint8_t)(0.5000 * r - 0.4187 * g - 0.0813 * b + 128);
}
