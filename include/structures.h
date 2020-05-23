#ifndef _STRUCTURES_H_
#define _STRUCTURES_H_

#define TAILLE_BLOC 8
#define TAILLE_PPM 2
#define NIVEAUX_GRIS "P5"

/*Définition d'un type énuméré 
 *pour les échantillonages*/
typedef enum ech {
	SIMPLE,
	HORIZONTAL,
	VERTICAL_HORIZONTAL
} ech;

/*Définition d'une image au format PPM
 *Contient les dimensions réelles de l'image
 *Contient les dimensions complétées de l'image
 *Contient la plage de couleurs
 *Contient le nombre de MCUs
 *Contient le format associé*/
typedef struct image_ppm {
	uint32_t largeur;
	uint32_t hauteur;
	uint32_t largeur_totale;
	uint32_t hauteur_totale;
	uint32_t nb_couleurs;
	uint32_t nb_MCUs;
	char format[TAILLE_PPM];
} image_ppm;

/*Définition de la composante Y*/
typedef struct Y {
	uint8_t** Y_downsampling;
} Y;

/*Définition des MCUs
 *Contient la hauteur et la largeur des MCUs
 *Contient le numéro ligne/colonne du MCU courant
 *Contient l'échantillonage
 *Contient le nombre de blocs de luminance 8x8
 *Contient les composantes Y, Cb, Cr
 *Contient les composantes Y, Cb, Cr après downsampling*/
typedef struct MCUs {
	uint32_t largeur;
	uint32_t hauteur;
	uint32_t numero_ligne;
	uint32_t numero_colonne;
	ech echantillonage;
	uint32_t nb_blocs_Y;
	uint8_t** Y;
	uint8_t** Cb;
	uint8_t** Cr;
	Y* comp_Y;
	uint8_t** Cb_downsampling;
	uint8_t** Cr_downsampling;
} MCUs;

#endif
