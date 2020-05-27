#ifndef _ZIGZAG_H
#define _ZIGZAG_H

#include <stdint.h>

/* type : int16_t[8][8], int16_t[64]
 * rtype : void
 * L'opération de réorganisation en "zig-zag" permet de représenter un
 * bloc 8 x 8 sous forme de vecteur 1D de 64 coefficients
 */
extern void zigzag(int16_t matrice[8][8], int16_t tableau[64]);

/* type : int16_t[8][8]
 * rtype : void
 * Réorganisation en "zigzag" en place dans la matrice 8x8 de départ
 */
extern void zigzag_inplace(int16_t matrice[8][8]);

/* type : int16_t[8][8]
 * rtype : void
 * Affichage de la matrice zigzag
 */
extern void afficher_zigzag(const int16_t zigzag[8][8]);
#endif
