#include "echantillonage.h"

extern ech recuperer_echantillonage(uint8_t h, uint8_t v) {
	/*Spécifiée uniquement pour les types du sujet
	 *pour le moment*/
	return (h == 4) ? SIMPLE : (v == 2) ? HORIZONTAL : VERTICAL_HORIZONTAL;
}

extern MCUs* recuperer_dimensions(ech echantillon) {
	MCUs* bloc = malloc(sizeof(MCUs));
	if (bloc == NULL) exit(EXIT_FAILURE);
		
	switch(echantillon) {
		case HORIZONTAL: /*4:2:2*/
			bloc->hauteur = TAILLE_BLOC;
		    bloc->largeur = 2 * TAILLE_BLOC;	
			break;
		case VERTICAL_HORIZONTAL: /*4:2:0*/
			bloc->hauteur = 2 * TAILLE_BLOC;
			bloc->largeur = 2 * TAILLE_BLOC;
			break;
		default: /*4:4:4*/
			bloc->hauteur = TAILLE_BLOC;
			bloc->largeur = TAILLE_BLOC;
			break;
	}

	return bloc;
}

extern uint32_t recuperer_hauteur(MCUs* bloc, uint32_t hauteur) {
	uint32_t verticale = hauteur / bloc->hauteur;

	/*Cas où la hauteur n'est pas un multiple de la hauteur
	 * des MCUs*/
	if (hauteur % bloc->hauteur != 0) {
		verticale = (uint32_t)verticale + 1;
	}

	/*Nombre total de lignes*/
	return verticale * bloc->hauteur;
}

extern uint32_t recuperer_largeur(MCUs* bloc, uint32_t largeur) {
	uint32_t horizontale = largeur / bloc->largeur;

	/*Cas où la largeur n'est pas un multiple de la largeur
	 * des MCUs*/
	if (largeur % bloc->largeur != 0) {
		horizontale = (uint32_t)horizontale + 1;
	}

	/*Nombre total de colonnes*/
	return horizontale * bloc->largeur;
}
