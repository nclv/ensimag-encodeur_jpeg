#include "compression.h"

bool compression(char* nom_fichier) {
	/*Ouverture du fichier*/
	FILE* fichier = ouvrir_fichier(nom_fichier);

	/*Parsing en-tête*/
	image_ppm* image = parse_entete(fichier);

	MCUs* bloc = initialiser_MCUs(image);

	printf("Largeur image : %d\n", image->largeur);
	printf("Hauteur image : %d\n", image->hauteur);
	printf("Largeur totale : %d\n", image->largeur_totale);
	printf("Hauteur totale : %d\n", image->hauteur_totale);
	printf("Plage de couleurs : %d\n", image->nb_couleurs);
	printf("Nombre de MCUs : %d\n", image->nb_MCUs);
	printf("Format : %2s\n", image->format);

	printf("Largeur_bloc = %d\n", bloc->largeur);
	printf("Hauteur_bloc = %d\n", bloc->hauteur);


	/*Traitement des MCUs*/
	for (size_t i = 0; i < image->nb_MCUs; i++) {
		printf("Traitement du bloc %ld\n", i + 1);
		recuperer_MCUs(fichier, image, bloc);
		afficher_MCUs(image, bloc);
	}

	/*Fermeture du fichier*/
	fermer_fichier(fichier);

	return true;
}
