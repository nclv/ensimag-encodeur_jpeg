#include <stdio.h>
#include <stdlib.h>

#include "downsampling.h"

int main(int argc, char** argv) {
    // process_Y(mcu->Y, sampling_factors[Y][H], sampling_factors[Y][V], data_unit);
    // process_chroma(mcu->Cb, sampling_factors[Y][H], sampling_factors[Y][V], sampling_factors[Cb][H], sampling_factors[Cb][V], data_unit);
    // process_chroma(mcu->Cr, sampling_factors[Y][H], sampling_factors[Y][V], sampling_factors[Cr][H], sampling_factors[Cr][V], data_unit);

    /* Allocation d'un bloc 8x8 */
    int16_t** data_unit = calloc(8, sizeof *data_unit);
    if (data_unit == NULL) exit(EXIT_FAILURE);
    for (size_t i = 0; i < 8; i++) {
        data_unit[i] = calloc(8, sizeof *data_unit[i]);
        if (data_unit[i] == NULL) exit(EXIT_FAILURE);
    }


    /* Libération d'un bloc 8x8 */
    for (size_t i = 0; i < 8; i++) {
        free(data_unit[i]);
    }
    free(data_unit);

    return EXIT_SUCCESS;
}