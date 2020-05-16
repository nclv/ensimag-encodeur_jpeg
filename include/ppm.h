#ifndef _PPM_H_
#define _PPM_H_

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "echantillonage.h"

#define TAILLE_PPM 2
#define NIVEAUX_GRIS "P5"

/*Définition d'une structure pour un pixel
 *Pour la sémantique et éviter d'utiliser des triples pointeurs*/
typedef struct pixel {
	uint8_t* couleurs;
} pixel;

/* Définition et déclaration d'une image ppm*/
typedef struct image_ppm {
	pixel** matrice;
	uint32_t hauteur;
	uint32_t largeur;
	uint32_t nb_couleurs;
	char mode[TAILLE_PPM];
} image_ppm;

/*
 * type: uint32_t
 * type: uint32_t
 * type: uint32_t
 * rtype: pixel**
 * Initialise une matrice de pixels avec les hauteurs et largeurs TOTALES
 * Initialise la taille des couleurs (1 pour P5, 3 pour P6)
 */
extern pixel** initialiser_matrice(uint32_t hauteur, uint32_t largeur, uint32_t nb_couleurs); 

/*
 * type: char[]
 * rtype: bool
 * Choisit le mode de remplissage, P5 ou P6
 */
extern bool choix_remplissage(char mode[]);

/*
 * type: image_ppm*
 * type: FILE*
 * rtype: void
 * Lit les octets de l'image format P5
 */
extern void parse_p5(image_ppm* image, FILE* fichier);

/*
 * type: image_ppm*
 * type: FILE*
 * rtype: void
 * Lit les octets de l'image format P6
 */
extern void parse_p6(image_ppm* image, FILE* fichier);

/* 
 * type: char*
 * rtype: image_ppm*
 * Lit et stocke une image au format ppm
 * Conversion en format YCbCr
 * Complétion des MCUs
 */
extern image_ppm* lire_ppm(char* nom_fichier, uint8_t h, uint8_t v);

/*
 * type: image_ppm*
 * type: uint32_t
 * type: uint32_t
 * rtype: void
 * Complète l'image en fonction des dimensions des MCUs
 * Et du type de l'image P5
 */
extern void completer_image_p5(image_ppm* image, uint32_t hauteur, uint32_t largeur);

/*
 * type: image_ppm*
 * type: uint32_t
 * type: uint32_t
 * rtype: void
 * Complète l'image en fonction des dimensions des MCUs
 * Et du type de l'image P6
 */
extern void completer_image_p6(image_ppm* image, uint32_t hauteur, uint32_t largeur);

/*
 * type: image_ppm*
 * rtype: void
 * Affiche une image au format ppm en format YCbCr
 * Format hexadecimal
 */
extern void afficher_image(image_ppm* image);

/*
 * type: image_ppm*
 * rtype: void
 * Libère l'image au format ppm
 */
extern void liberer_image(image_ppm* image);

#endif
