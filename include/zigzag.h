#ifndef _ZIGZAG_H
#define _ZIGZAG_H

#include <stdint.h>

static uint8_t indices_zigzag[64] = {
    0, 1, 8, 16, 9, 2, 3, 10,
    17, 24, 32, 25, 18, 11, 4, 5,
    12, 19, 26, 33, 40, 48, 41, 34,
    27, 20, 13, 6, 7, 14, 21, 28,
    35, 42, 49, 56, 57, 50, 43, 36,
    29, 22, 15, 23, 30, 37, 44, 51,
    58, 59, 52, 45, 38, 31, 39, 46,
    53, 60, 61, 54, 47, 55, 62, 63};

/* type : int16_t
 * rtype : vecteur de int16_t
 * L'opération de réorganisation en "zig-zag" permet de représenter un
 * bloc 8 x 8 sous forme de vecteur 1D de 64 coefficients
 */
extern void zigzag(int16_t matrice[8][8], int16_t tableau[64]);

extern void zigzag_inplace(int16_t matrice[8][8]);

#endif
