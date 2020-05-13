#ifndef _PPM_H
#define _PPM_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*Définition et déclaration d'un pixel de couleurs RGB*/
typedef struct pixel {
    uint8_t triplet_rgb[3];
    uint8_t triplet_ycbcr[3];
} pixel;

/*Définition et déclaration d'une image ppm*/
typedef struct image_ppm {
    pixel** matrice;
    uint32_t hauteur;
    uint32_t largeur;
    uint32_t nb_couleurs;
    char mode[2];
} image_ppm;

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
