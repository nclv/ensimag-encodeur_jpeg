#include "echantillonage.h"

ech recuperer_echantillonage(uint8_t h, uint8_t v) {
	/*Spécifiée uniquement pour les types du sujet
	 *pour le moment*/
	return (h == 4) ? SIMPLE : (v == 2) ? HORIZONTAL : VERTICAL_HORIZONTAL;
}
