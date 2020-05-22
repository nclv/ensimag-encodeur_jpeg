#ifndef _COMPRESSION_H_
#define _COMPRESSION_H_

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include "ppm.h"
#include "echantillonage.h"

/* type: char*
 * type: uint8_t
 * type: uint8_t
 * rtype: bool
 * Compresse l'image au format ppm
 * Renvoie True si la compression s'est bien effectuée
 * False sinon
 */
extern bool compression(char* nom_fichier, uint8_t h, uint8_t v);

#endif
