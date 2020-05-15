#ifndef _QUANTIFICATION_H
#define _QUANTIFICATION_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



/*
 * type: uint8_t* de taille 64 (au format zigzag), uint8_t flag indiquant le type de composante
   que l'on veut quantifier
 * rtype: uint8_t* de taille 64 (au format zigzag)
 * Creer et retourne le bloc zigzag quantifie
*/
extern uint8_t* quantifier (uint8_t* bloc_zigzag_a_quantifier, uint8_t flag_composante);

#endif
