#include "mcus.h"

MCUs* recuperer_blocs(image_ppm* image) {
	MCUs* matrice = (MCUs*) malloc(sizeof(MCUs));
	uint32_t verticale = image->hauteur / TAILLE_BLOC;
	uint32_t horizontale = image->largeur / TAILLE_BLOC;

	if (image->hauteur % TAILLE_BLOC != 0) {
		verticale = uint32_t(verticale) + 1;
	}
	if (image->largeur % TAILLE_BLOC != 0) {
		horizontale = uint32_t(horizontale) + 1;
	}

	matrice->nombre_blocs = uint32_t(verticale * horizontale); 
	matrice->blocs = (uint32_t**) malloc(nombre_blocs * sizeof(uint32_t*));
	for (uint32_t i = 0; i < matrice->nombre_blocs; i++) {
		matrice->blocs[i] = (uint32_t*) malloc(2 * sizeof(uint32_t));
	}

	for (uint32_t i = 0; i < verticale; i++) {
		for (uint32_t j = 0; j < horizontale; j++) {
			matrice->blocs[i][j][0] = i * 8;
			matrice->blocs[i][j][1] = j * 8;
		}
	}
	
	return matrice;
}

