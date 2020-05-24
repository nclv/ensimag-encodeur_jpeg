#ifndef _PPM_H_
#define _PPM_H_

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

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

/* type: char
 * rtype: bool
 * Choisit le mode de remplissage P5 ou P6
 */
extern bool choisir_format(char format[]);

/* type: image_ppm*
 * rtype: MCUs*
 * Initialise les MCUs en fonction de l'échantillonage
 * et des dimensions de l'image
 */
extern MCUs* initialiser_MCUs(image_ppm* image, uint8_t facteurs[NOMBRE_FACTEURS]);

/* type: MCUs*
 * type: uint32_t
 * rtype: uint32_t
 * Récupère la largeur des MCUs
 */
extern uint32_t recuperer_largeur(MCUs* bloc, uint32_t largeur);

/* type: MCUs*
 * type: uint32_t
 * rtype: uint32_t
 * Récupère la hauteur des MCUs
 */
extern uint32_t recuperer_hauteur(MCUs* bloc, uint32_t hauteur);

/* type: FILE*
 * type: image_ppm*
 * type: MCUs*
 * rtype: void
 * Récupère les MCUs à la volée un par un
 */
extern void recuperer_MCUs(FILE* fichier, image_ppm* image, MCUs* bloc);

/* type: MCUs*
 * type: uint32_t
 * type: uint32_t
 * rtype: void
 * Complète en largeur et en hauteur le MCU donné
 * Pour un format P5
 */
extern void completer_grayscale(MCUs* bloc, uint32_t largeur, uint32_t hauteur);

/* type: MCUs*
 * type: uint32_t
 * type: uint32_t
 * rtype: void
 * Complète en largeur et en hauteur le MCU donné
 * Pour un format P6
 */
extern void completer_RGB(MCUs* bloc, uint32_t largeur, uint32_t hauteur);

/* type: FILE*
 * type: image_ppm*
 * type: MCUs*
 * type: uint32_t
 * type: uint32_t
 * rtype: void
 * Parse l'image en P5 (MCU par MCU à la volée)
 */
extern void parse_grayscale(FILE* fichier, image_ppm* image, MCUs* bloc, uint32_t largeur, uint32_t hauteur);

/* type: FILE*
 * type: image_ppm*
 * type: MCUs*
 * type: uint32_t
 * type: uint32_t
 * rtype: void
 * Parse l'image en P6 (MCU par MCU à la volée)
 */
extern void parse_RGB(FILE* fichier, image_ppm* image, MCUs* bloc, uint32_t largeur, uint32_t hauteur);

/* type: image_ppm*
 * type: MCUs*
 * rtype: void
 * Affiche un MCU en héxadécimal en fonction de P5 ou P6
 */
extern void afficher_MCUs(image_ppm* image, MCUs* bloc);

#endif
