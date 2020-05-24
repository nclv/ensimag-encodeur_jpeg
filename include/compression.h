#ifndef _COMPRESSION_H_
#define _COMPRESSION_H_

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

/* type: char*
 * rtype: bool
 * Compresse l'image au format ppm
 * Renvoie True si la compression s'est bien effectuée
 * False sinon
 */
extern bool compression(char* nom_fichier);

#endif
