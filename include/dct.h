#ifndef _DCT_H
#define _DCT_H

#include <stdint.h>

/* type: int16_t**, int16_t[8][8]
 * rtype: void
 * Applique la dct à une matrice d'entiers 16 bits
 * et écrit le résultat dans la matrice d'entiers
 * 16 bits 8x8 output
 */
extern void dct(int16_t **input, int16_t output[8][8]);

/* type: int16_t**
 * rtype: void
 * Réalise un décalage de 128 sur toutes les cases de
 * la matrice input
 */
extern void offset(int16_t **input);

#endif
