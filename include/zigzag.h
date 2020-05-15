#ifndef _ZIGZAG_H
#define _ZIGZAG_H

#include <stdio.h>
#include <stdlib.h>

// tableau en sortie de la fonction zigzag
typedef struct tableau {
  uint8_t tableau[64];
} tableau;

// bloc 8x8
typedef struct bloc {
  uint8_t bloc[8][8];
} bloc;

/* type : uint8_t
 * rtype : vecteur de uint8_t
 * L'opération de réorganisation en "zig-zag" permet de représenter un
 * bloc 8 x 8 sous forme de vecteur 1D de 64 coefficients
 */
extern tableau zigzag1(bloc matrice);

#endif
