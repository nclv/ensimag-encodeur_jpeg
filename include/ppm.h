#ifndef _PPM_H_
#define _PPM_H_

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define TAILLE_BLOC 8
#define TAILLE_PPM 2
#define NIVEAUX_GRIS "P5"

/* Type énuméré représentant les composantes de couleur YCbCr. */
enum color_component {
    Y,
    Cb,
    Cr,
    NB_COLOR_COMPONENTS
};

/*
    Type énuméré représentant la direction des facteurs d'échantillonnage (H
    pour horizontal, V pour vertical).
*/
enum direction {
    H,
    V,
    NB_DIRECTIONS
};

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

/*Définition des MCUs*/
typedef struct MCUs {
    uint32_t largeur;
    uint32_t hauteur;
    uint32_t numero_ligne;
    uint32_t numero_colonne;
    uint32_t nb_blocs_Y;
    uint32_t nb_blocs_Cb;
    uint32_t nb_blocs_Cr;
    int16_t** Y;
    int16_t** Cb;
    int16_t** Cr;
} MCUs;

/* type: char*
 * rtype: FILE*
 * Ouvre le fichier PPM/PGM
 */
extern FILE* ouvrir_fichier(char* nom_fichier);

/* type: FILE*
 * rtype: void
 * Ferme le fichier PPM/PGM
 */
extern void fermer_fichier(FILE* fichier);

/* type: FILE*
 * rtype: image_ppm*
 * Parse l'en-tête  du fichier PPM/PGM
 * Stocke les informations dans une structure image_ppm
 */
extern image_ppm* parse_entete(FILE* fichier);

/* type: image_ppm*
 * rtype: MCUs*
 * Initialise les MCUs en fonction de l'échantillonage
 * et des dimensions de l'image
 */
extern MCUs* initialiser_MCUs(image_ppm* image, uint8_t sampling_factors[NB_COLOR_COMPONENTS][NB_DIRECTIONS]);


/* type: FILE*
 * type: image_ppm*
 * type: MCUs*
 * rtype: void
 * Récupère les MCUs à la volée un par un
 */
extern void recuperer_MCUs(FILE* fichier, image_ppm* image, MCUs* bloc);

/* type: image_ppm*
 * type: MCUs*
 * rtype: void
 * Affiche un MCU en héxadécimal en fonction de P5 ou P6
 */
extern void afficher_MCUs(char format[TAILLE_PPM], MCUs* bloc);

#endif
