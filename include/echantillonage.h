#ifndef _ECHANTILLONAGE_H_
#define _ECHANTILLONAGE_H_

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define TAILLE_BLOC 8

/*Définition de l'échantillonage
 * SIMPLE 4:4:4
 * HORIZONTAL 4:2:2
 * VERTICAL_HORIZONTAL 4:2:0
 */
typedef enum ech {
	SIMPLE,
	HORIZONTAL,
	VERTICAL_HORIZONTAL
} ech;	

/*Définition des MCUs*/
typedef struct MCUs {
	uint32_t hauteur;
	uint32_t largeur;
} MCUs;

/*
 * type: uint8_t
 * type: uint8_t
 * rtype: ech
 * Détermine l'échantillonage suivant la valeur de h et v
 */
extern ech recuperer_echantillonage(uint8_t h, uint8_t v);

/*
 * type: ech
 * rtype: MCUs*
 * Récupère les dimensions des MCUs suivant l'échantillonage
 */
extern MCUs* recuperer_dimensions(ech echantillon);

/*
 * type: MCUs*
 * type: uint32_t
 * rtype: uint32_t
 * Récupère la hauteur totale connaissant les dimensions
 * des MCUs et la hauteur de l'image
 */
extern uint32_t recuperer_hauteur(MCUs* bloc, uint32_t hauteur);

/*
 * type: MCUs*
 * type uint32_t
 * rtype: uint32_t
 * Récupère la largeur totale connaissant les dimensions
 * des MCUs et la largeur de l'image
 */
extern uint32_t recuperer_largeur(MCUs* bloc, uint32_t largeur);

#endif
