#ifndef _QUANTIFICATION_H_
#define _QUANTIFICATION_H_

#include <stdint.h>

/*
 * type: uint8_t* de taille 64 (au format zigzag), uint8_t flag indiquant le type de composante
   que l'on veut quantifier
 * rtype: uint8_t* de taille 64 (au format zigzag)
 * Creer et retourne le bloc zigzag quantifie
*/
extern void quantifier(int16_t matrice_frequentiel_zigzag[8][8], uint8_t quantification_table[64]);

/*
 * type: int16_t[8][8]
 * rtype: void
 * Affiche la matrice quantifiee
*/
extern void afficher_matrice_quantifiee(int16_t matrice_quantifiee[8][8]);

#endif /* _QUANTIFICATION_H_ */
