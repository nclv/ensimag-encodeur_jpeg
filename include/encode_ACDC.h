#ifndef _ENCODE_ACDC_H
#define _ENCODE_ACDC_H

#include <stdint.h>

/*
 * type: deux uint8_t -- DC courante et précédente
 * rtype: uint32_t valeur d'encodage de DC
 * Encode la valeur DC de la MCU courante et renvoie la valeur encodée
*/
extern void encoder_DC_freq(int8_t difference_DC);

/*
 * type: uint8_t[8][8] -- MCU courante
 * rtype: uint32_t valeur d'encodage de AC
 * Encode la valeur AC de la MCU courante et renvoie la valeur encodée
*/
extern void encoder_AC_freq(int8_t freq_AC, uint8_t nb_zeros_enchaines);

#endif
