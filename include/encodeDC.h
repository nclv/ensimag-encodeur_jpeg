#ifndef _ENCODEDC_H
#define _ENCODEDC_H

#include <stdint.h>

/*
 * type: deux uint8_t -- DC courante et précédente
 * rtype: uint32_t valeur d'encodage de DC
 * Encode la valeur DC de la MCU courante et renvoie la valeur encodée
*/
extern uint32_t encoderDC(int8_t MCU_courante_DC, int8_t MCU_ancienne_DC);

#endif
