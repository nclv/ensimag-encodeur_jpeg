#ifndef _ENCODE_ACDC_H
#define _ENCODE_ACDC_H

#include <stdint.h>

/*
 * type: deux uint8_t -- DC courante et précédente
 * rtype: uint32_t valeur d'encodage de DC
 * Encode la valeur DC de la MCU courante et renvoie la valeur encodée
*/
extern uint32_t encoder_DC_freq(int8_t difference_DC);

/*
 * type: uint8_t[8][8] -- MCU courante
 * rtype: uint32_t valeur d'encodage de AC
 * Encode la valeur AC de la MCU courante et renvoie la valeur encodée
*/
extern uint32_t encoderAC(int8_t MCU_courante[8][8]);

#endif
