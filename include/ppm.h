#ifndef _PPM_H_
#define _PPM_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "echantillonage.h"
#include "structures.h"

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
extern bool choix_remplissage(char format[]);

/* type: image_ppm*
 * type: ech
 * rtype: MCUs*
 * Initialise les MCUs en fonction de l'échantillonage
 * et des dimensions de l'image
 */
extern MCUs* initialiser_MCUs(image_ppm* image, ech echantillonage);

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
extern void completer_bloc_P5(MCUs* bloc, uint32_t largeur, uint32_t hauteur);

/* type: MCUs*
 * type: uint32_t
 * type: uint32_t
 * rtype: void
 * Complète en largeur et en hauteur le MCU donné
 * Pour un format P6
 */
extern void completer_bloc_P6(MCUs* bloc, uint32_t largeur, uint32_t hauteur);

/* type: FILE*
 * type: image_ppm*
 * type: MCUs*
 * type: uint32_t
 * type: uint32_t
 * rtype: void
 * Parse l'image en P5 (MCU par MCU à la volée)
 */
extern void parse_P5(FILE* fichier, image_ppm* image, MCUs* bloc, uint32_t largeur, uint32_t hauteur);

/* type: FILE*
 * type: image_ppm*
 * type: MCUs*
 * type: uint32_t
 * type: uint32_t
 * rtype: void
 * Parse l'image en P6 (MCU par MCU à la volée)
 */
extern void parse_P6(FILE* fichier, image_ppm* image, MCUs* bloc, uint32_t largeur, uint32_t hauteur);

/* type: image_ppm*
 * rtype: void
 * Affiche les informations de l'image
 */
extern void afficher_image(image_ppm* image);

/* type: MCUs*
 * type: bool
 * rtype: void
 * Affiche un MCU en héxadécimal en fonction de P5 ou P6
 */
extern void afficher_MCUs(MCUs* bloc, bool choix);

#endif
