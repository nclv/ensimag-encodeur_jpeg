#ifndef _ENCODEAC_H
#define _ENCODEAC_H

#include <stdint.h>


/*
 * type: uint8_t[8][8] -- MCU courante
 * rtype: uint32_t valeur d'encodage de AC
 * Encode la valeur AC de la MCU courante et renvoie la valeur encodée
*/
extern uint32_t encoderAC(uint8_t MCU_courante[8][8]);

#endif
