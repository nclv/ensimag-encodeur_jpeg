#ifndef _ECHANTILLONAGE_H_
#define _ECHANTILLONAGE_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "ppm.h"
#include "structures.h"

/* type: uint8_t
 * type: uint8_t
 * rtype: ech
 * Récupère l'échantillonage
 */
extern ech recuperer_echantillonage(uint8_t h, uint8_t v);

/* type: ech
 * rtype: bool
 * Renvoie true si on réalise le downsampling
 * Sinon false
 */
extern bool realiser_downsampling(ech echantillonage);

/* type: MCUs*
 * rtype: void
 * Initialise les composantes pour le downsampling
 */
extern void initialiser_composantes(MCUs* bloc);

/* type: size_t
 * type: size_t
 * rtype: size_t
 * Renvoie l'indice de la matrice de luminance
 * Utilisée pour le downsampling
 */
extern size_t choix_indice(size_t i, size_t j);

/* type: MCUs*
 * rtype: void
 * Effectue le downsampling
 */
extern void downsampling(MCUs* bloc);

/* type: MCUs*
 * rtype: void
 * Sous-échantillone un MCU horizontalement
 */
extern void compresser_horizontal(MCUs* bloc);

/* type: MCUs*
 * rtype: void
 * Sous-échantillone un MCU verticalement et horizontalement
 */
extern void compresser_vertical_horizontal(MCUs* bloc);

/* type: MCUs*
 * rtype: void
 * Affiche les MCUs après downsampling
 */
extern void afficher_MCUs_Downsampling(MCUs* bloc);

#endif
