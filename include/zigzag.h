#ifndef _ZIGZAG_H
#define _ZIGZAG_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* type : int16_t
 * rtype : vecteur de int16_t
 * L'opération de réorganisation en "zig-zag" permet de représenter un
 * bloc 8 x 8 sous forme de vecteur 1D de 64 coefficients
 */
extern void zigzag(int16_t matrice[8][8], int16_t tableau[64]);

#endif
