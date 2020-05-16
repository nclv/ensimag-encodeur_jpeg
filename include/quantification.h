#ifndef _QUANTIFICATION_H_
#define _QUANTIFICATION_H_

#include <stdint.h>

/*
 * type: uint8_t* de taille 64 (au format zigzag), uint8_t flag indiquant le type de composante
   que l'on veut quantifier
 * rtype: uint8_t* de taille 64 (au format zigzag)
 * Creer et retourne le bloc zigzag quantifie
*/
extern uint8_t* quantifier(uint8_t* bloc_zigzag_a_quantifier, uint8_t flag_composante);

#endif /* _QUANTIFICATION_H_ */