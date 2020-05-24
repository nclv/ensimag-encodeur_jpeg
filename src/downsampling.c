#include "downsampling.h"

bool verifier_facteurs() {
	return (facteurs[H1] >= 1 && facteurs[H1] <= 4 &&
			 facteurs[H2] >= 1 && facteurs[H2] <= 4 &&
		 	 facteurs[H3] >= 1 && facteurs[H3] <= 4 &&
			 facteurs[V1] >= 1 && facteurs[V1] <= 4 &&
			 facteurs[V2] >= 1 && facteurs[V2] <= 4 &&
			 facteurs[V3] >= 1 && facteurs[V3] <= 4 &&
			 (facteurs[H1] * facteurs[V1] +
		 	  facteurs[H2] * facteurs[V2] +
			  facteurs[H3] * facteurs[V3]) <= 10 &&
	   		 (facteurs[H1] % facteurs[H2] == 0) &&
			 (facteurs[H1] % facteurs[H3] == 0) &&
			 (facteurs[V1] % facteurs[V2] == 0) &&
			 (facteurs[V1] % facteurs[V3] == 0));
}

void initialiser_MCUs_downsampling(MCUs* bloc) {
	bloc->nb_blocs_Y = facteurs[H1] * facteurs[V1];
	bloc->nb_blocs_Cb = facteurs[H2] * facteurs[V2];
	bloc->nb_blocs_Cr = facteurs[H3] * facteurs[V3];

	bloc->comp_Y = malloc(bloc->nb_blocs_Y * sizeof(Y));
	bloc->comp_Cb = malloc(bloc->nb_blocs_Cb * sizeof(Cb));
	bloc->comp_Cr = malloc(bloc->nb_blocs_Cr * sizeof(Cr));

	for (size_t i = 0; i < bloc->nb_blocs_Y; i++) {
		bloc->comp_Y[i]->Y_downsampling = malloc(TAILLE_BLOC * sizeof(uint8_t*));
		for (size_t j = 0; j < TAILLE_BLOC; j++) {
			bloc->comp_Y[i]->Y_downsampling[i] = malloc(TAILLE * sizeof(uint8_t));
		}
	}

	for (size_t i = 0; i < bloc->nb_blocs_Cb; i++) {
		bloc->comp_Cb[i]->Cb_downsampling = malloc(TAILLE_BLOC * sizeof(uint8_t*));
		for (size_t j = 0; j < TAILLE_BLOC; j++) {
			bloc->comp_Cb[i]->Cb_downsampling[j] = malloc(TAILLE_BLOC * sizeof(uint8_t));
		}
	}

	for (size_t i = 0; i < bloc->nb_blocs_Cr; i++) {
		bloc->comp_Cr[i]->Cr_downsampling = malloc(TAILLE_BLOC * sizeof(uint8_t*));
		for (size_t j = 0; j < TAILLE_BLOC; j++) {
			bloc->comp_Cr[i]->Cr_downsampling[j] = malloc(TAILLE_BLOC * sizeof(uint8_t));
		}
	}
}

void downsampling(MCUs* bloc) {
		
}
