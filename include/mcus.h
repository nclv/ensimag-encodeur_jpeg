#ifndef _MCUS_H_
#define _MCUS_H_

#define TAILLE_BLOC 8

#include "ppm.h"

/*Définition des MCUs*/
typedef struct MCUs MCUs;
struct MCUs {
	uint32_t** blocs; 		
	uint32_t nombre_blocs;
};

/*
 * type: image_ppm*
 * rtype: MCUs*
 * Renvoie les MCUs sous formes de blocs 
 * On retient l'indice du premier élément de chaque blocs dans l'image
 * Complexité mémoire réduite
 */
MCUs* recuperer_blocs(image_ppm* image);

/*
 * type: MCU*s
 * rtype: void
 * Affiche la matrice des indices des blocs
 */
void afficher_MCUs(MCUs* matrice);

#endif
