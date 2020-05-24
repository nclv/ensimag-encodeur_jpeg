#ifndef _STRUCTURES_H_
#define _STRUCTURES_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define TAILLE_BLOC 8
#define TAILLE_PPM 2
#define NIVEAUX_GRIS "P5"
#define NOMBRE_FACTEURS 6
#define H1 0
#define V1 1
#define H2 2
#define V2 3
#define H3 4
#define V3 5

/*Définition des facteurs d'échantillonage*/
extern uint8_t facteurs[NOMBRE_FACTEURS];

/*Définition d'une image au format PPM/PGM*/
typedef struct image_ppm {
	uint32_t largeur;
	uint32_t hauteur;
	uint32_t largeur_totale;
	uint32_t hauteur_totale;
	uint32_t nb_couleurs;
	uint32_t nb_MCUs;
	char format[TAILLE_PPM];
} image_ppm;

/*Définition des composantes Y*/
typedef struct Y {
	uint8_t** Y_downsampling;
} Y;

/*Définition des composantes Cb*/
typedef struct Cb {
	uint8_t** Cb_downsampling;
} Cb;

/*Définition des composants Cr*/
typedef struct Cr {
	uint8_t** Cr_downsampling;
} Cr;

/*Définition des MCUs*/
typedef struct MCUs {
	uint32_t largeur;
	uint32_t hauteur;
	uint32_t numero_ligne;
	uint32_t numero_colonne;
	uint32_t nb_blocs_Y;
	uint32_t nb_blocs_Cb;
	uint32_t nb_blocs_Cr;
	uint8_t** Y;
	uint8_t** Cb;
	uint8_t** Cr;
	Y* comp_Y;
	Cb* comp_Cb;
	Cr* comp_Cr;
} MCUs;

#endif
