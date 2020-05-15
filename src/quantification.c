#include "qtables.h"
#include "quantification.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


uint8_t* quantifier (uint8_t* bloc_zigzag_a_quantifier, uint8_t flag_composante) {
    uint8_t* quantified_zigzag = malloc(64 * sizeof(uint8_t));
    if (flag_composante == 0) { // Y
      for (uint8_t i=0;i<64;i++) {
        quantified_zigzag[i] = (uint8_t) ((float)bloc_zigzag_a_quantifier[i]/quantification_table_Y[i]);
      }
    }
    else { // Cb ou Cr
      for (uint8_t i=0;i<64;i++) {
        quantified_zigzag[i] = (uint8_t) ((float)bloc_zigzag_a_quantifier[i]/quantification_table_CbCr[i]);
      }
    }
    return quantified_zigzag;
}
