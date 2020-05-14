#ifndef _DCT_H
#define _DCT_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Définition et déclaration d'une matrice de cosinus pour la dtc d'indices (i,j)
   plus facile pour créer une matrice contenant toutes les matrice_cosinus_dtc
*/
typedef struct matrice_cosinus_dct {
  float** matrice;
} matrice_cosinus_dct;

/*
 * type: trois uint8_t
 * rtype: matrice_cosinus_dct*
 * Creer et retourne la matrice des cosinus necessaires a la dtc d'indices (i,j)
*/
extern matrice_cosinus_dct* creer_matrice_i_j(uint8_t i, uint8_t j, uint8_t n);

/*
 * type: uint8_t
 * rtype: matrice_cosinus_dct**
 * Creer une matrice contenant nxn matrices de cosinus pour la dtc telle que
   tous les indices soient parcourus
*/
extern matrice_cosinus_dct** creer_matrices_dtc(uint8_t n);


#endif