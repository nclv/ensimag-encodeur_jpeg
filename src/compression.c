#include "compression.h"

bool compression(char* nom_fichier, uint8_t h, uint8_t v) {
	/*Ouverture du fichier*/
	FILE* fichier = ouvrir_fichier(nom_fichier);

	/*Parsing en-tête*/
	image_ppm* image = parse_entete(fichier);

	/*Choix du sous-échantillonage*/
	ech echantillonage;
	if (choix_remplissage(image->format)) {
		echantillonage = SIMPLE;
	}
	else {
		echantillonage = recuperer_echantillonage(h, v);
	}

	MCUs* bloc = initialiser_MCUs(image, echantillonage);

	/*Traitement des MCUs*/
	for (size_t i = 0; i < image->nb_MCUs; i++) {
		printf("Traitement du bloc %ld\n", i + 1);
		recuperer_MCUs(fichier, image, bloc);
		if (choix_remplissage(image->format)) {
			afficher_MCUs(bloc, true);
		}
		else {
			afficher_MCUs(bloc, false);
		}
		if (!choix_remplissage(image->format) && realiser_downsampling(bloc->echantillonage)) {
			initialiser_composantes(bloc);
			downsampling(bloc);
			afficher_MCUs_Downsampling(bloc);
		}
	}

	/*Fermeture du fichier*/
	fermer_fichier(fichier);

	return true;
}
