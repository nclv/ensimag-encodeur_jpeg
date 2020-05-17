#include "encodeDC.h"
#include <math.h>
#include <stdint.h>
#include <string.h>

uint32_t encoderDC(uint8_t MCU_courante_DC, uint8_t MCU_ancienne_DC) {
  uint8_t valeur_a_encoder = MCU_courante_DC - MCU_ancienne_DC;
  uint8_t est_negatif = 0;
  if (valeur_a_encoder < 0) {
    valeur_a_encoder = -valeur_a_encoder;
    est_negatif = 1;
  }
  uint8_t classe_magnitude = ((uint8_t)(((string)log2(valeur_a_encoder))[0]))+1;
  
}
