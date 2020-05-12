#ifndef PPM_H
#define PPM_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define R 0
#define G 1
#define B 2

/*Définition d'un triplet de couleurs RGB*/
typedef struct triplet_rgb triplet_rgb;
struct triplet_rgb {
	uint8_t triplet[3];
};

/*Définition d'une image ppm*/
typedef struct image_ppm image_ppm;
struct image_ppm {
	triplet_rgb** matrice;
	uint32_t  hauteur;
	uint32_t largeur;
	uint32_t nb_couleurs;
	char mode[2];
};

/*
 * type: char*
 * rtype: image_ppm
 * Lit et stocke une image au format ppm
 */
image_ppm* lire_ppm(char* nom_fichier);

/*
 * type: image_ppm*
 * type: char*
 * rtype: void
 * Ecrit une image au format ppm dans un fichier
 * A faire plus tard
 */
void ecrire_ppm(image_ppm* image, char* nom_fichier);

/*
 * type: image_ppm*
 * rtype: void
 * Affiche une image au format ppm dans stdout
 */
void afficher_image(image_ppm* image);

/*
 * type: image_ppm*
 * rtype: void
 * Libere la place memoire de l'image
 * A faire plus tard
 */
//void liberer_image(image_ppm* image);

#endif
