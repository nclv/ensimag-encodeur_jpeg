#ifndef _ZIGZAG_H
#define _ZIGZAG_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* type : uint8_t
 * rtype : vecteur de uint8_t
 * L'opération de réorganisation en "zig-zag" permet de représenter un
 * bloc 8 x 8 sous forme de vecteur 1D de 64 coefficients
 */
extern uint8_t *zigzag(uint8_t matrice[8][8]);

#endif
