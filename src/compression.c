#include "compression.h"

bool compression(char* nom_fichier, uint8_t h, uint8_t v) {
	/*Ouverture du fichier*/
	FILE* fichier = ouvrir_fichier(nom_fichier);

	/*Parsing en-tête*/
	image_ppm* image = parse_entete(fichier);

	/*Choix du sous-échantillonage*/
	ech echantillonage = recuperer_echantillonage(h, v);
	MCUs* bloc = initialiser_MCUs(image, echantillonage);

	/*Traitement des MCUs*/
	for (size_t i = 0; i < image->nb_MCUs; i++) {
		printf("Traitement du bloc %ld\n", i + 1);
		recuperer_MCUs(fichier, image, bloc);
		afficher_MCUs(bloc, !choix_remplissage(image->format));
	}

	/*Fermeture du fichier*/
	fermer_fichier(fichier);

	return true;
}
