#include "ppm.h"
#include "conversion.h"

FILE* ouvrir_fichier(char* nom_fichier) {
	/*Ouverture du fichier*/
	FILE* fichier = fopen(nom_fichier, "r");
	if (fichier == NULL) exit(EXIT_FAILURE);

	return fichier;
}

void fermer_fichier(FILE* fichier) {
		fclose(fichier);
}

image_ppm* parse_entete(FILE* fichier) {
	/*Initialisation de l'image*/
	image_ppm* image = malloc(sizeof(image_ppm));
	if (image == NULL) exit(EXIT_FAILURE);

	/*Parsing en-tête*/
	int erreur_entete;
	erreur_entete = fscanf(fichier, "%2s", image->format);
	if (erreur_entete != 1) exit(EXIT_FAILURE);
	erreur_entete = fscanf(fichier, "%d", &image->largeur);
	if (erreur_entete != 1) exit(EXIT_FAILURE);
	erreur_entete = fscanf(fichier, "%d", &image->hauteur);
	if (erreur_entete != 1) exit(EXIT_FAILURE);
	erreur_entete = fscanf(fichier, "%d", &image->nb_couleurs);
	if (erreur_entete != 1) exit(EXIT_FAILURE);

	/*Parsing \n (de valeur 10)*/
	uint8_t octet;
	size_t erreur_backslash = fread(&octet, sizeof(uint8_t), 1, fichier);
	if (erreur_backslash != 1) exit(EXIT_FAILURE);

	return image;
}

bool choix_remplissage(char format[]) {
	return !strcmp(format, NIVEAUX_GRIS);
}

MCUs* initialiser_MCUs(image_ppm* image, ech echantillonage) {
	MCUs* bloc = malloc(sizeof(MCUs));
	if (bloc == NULL) exit(EXIT_FAILURE);

	switch(echantillonage) {
		case HORIZONTAL:
			bloc->largeur = 2 * TAILLE_BLOC;
			bloc->hauteur = TAILLE_BLOC;
			break;
		case VERTICAL_HORIZONTAL:
			bloc->largeur = 2 * TAILLE_BLOC;
			bloc->hauteur = 2 * TAILLE_BLOC;
			break;
		default:
			bloc->largeur = TAILLE_BLOC;
			bloc->hauteur = TAILLE_BLOC;
	}

	bloc->echantillonage = echantillonage;

	bloc->Y = malloc(bloc->hauteur * sizeof(uint8_t*));

	/*On traite les boucles dans les if
	 *c'est plus performant que de tout traiter en
	 *même temps. On évite les if dans les boucles
	 */
	if (choix_remplissage(image->format)) {
		bloc->Cb = NULL;
		bloc->Cr = NULL;
		for (size_t i = 0; i < bloc->hauteur; i++) {
			bloc->Y[i] = malloc(bloc->largeur * sizeof(uint8_t));
		}
	}
	else {
		bloc->Cb = malloc(bloc->hauteur * sizeof(uint8_t*));
		bloc->Cr = malloc(bloc->hauteur * sizeof(uint8_t*));
		for (size_t i = 0; i < bloc->hauteur; i++) {
			bloc->Y[i] = malloc(bloc->largeur * sizeof(uint8_t));
			bloc->Cb[i] = malloc(bloc->largeur * sizeof(uint8_t));
			bloc->Cr[i] = malloc(bloc->largeur * sizeof(uint8_t));
		}
	}

	if (echantillonage == SIMPLE) {
		bloc->comp_Y = NULL;
		bloc->Cb_downsampling = NULL;
		bloc->Cr_downsampling = NULL;
	}

	bloc->numero_ligne = 0;
	bloc->numero_colonne = 0;
	image->largeur_totale = recuperer_largeur(bloc, image->largeur);
	image->hauteur_totale = recuperer_hauteur(bloc, image->hauteur);
	image->nb_MCUs = (image->largeur_totale * image->hauteur_totale) / (bloc->largeur * bloc->hauteur);

	return bloc;
}

uint32_t recuperer_largeur(MCUs* bloc, uint32_t largeur) {
	uint32_t horizontale = largeur / bloc->largeur;

	/*Cas où la largeur n'est pas un multiple de la largeur
	 * des MCUs*/
	if (largeur % bloc->largeur != 0) {
		horizontale = (uint32_t)horizontale + 1;
	}

	/*Nombre total de colonnes*/
	return horizontale * bloc->largeur;
}


uint32_t recuperer_hauteur(MCUs* bloc, uint32_t hauteur) {
	uint32_t verticale = hauteur / bloc->hauteur;

	/*Cas où la hauteur n'est pas un multiple de la hauteur
	 * des MCUs*/
	if (hauteur % bloc->hauteur != 0) {
		verticale = (uint32_t)verticale + 1;
	}

	/*Nombre total de lignes*/
	return verticale * bloc->hauteur;
}

void recuperer_MCUs(FILE* fichier, image_ppm* image, MCUs* bloc) {
	uint32_t largeur;
	uint32_t hauteur;
	if (bloc->numero_colonne == (image->largeur_totale / bloc->largeur) - 1) {
		largeur = bloc->largeur - (image->largeur_totale - image->largeur);
	}
	else {
		largeur = bloc->largeur;
	}
	if (bloc->numero_ligne == (image->hauteur_totale / bloc->hauteur) - 1) {
		hauteur = bloc->hauteur - (image->hauteur_totale - image->hauteur);
	}
	else {
		hauteur = bloc->hauteur;
	}

	if (choix_remplissage(image->format)) {
		parse_P5(fichier, image, bloc, largeur, hauteur);
	}
	else {
		parse_P6(fichier, image, bloc, largeur, hauteur);
	}

	if (bloc->numero_colonne == (image->largeur_totale / bloc->largeur) - 1) {
		bloc->numero_ligne++;
		bloc->numero_colonne = 0;
	}
	else {
		bloc->numero_colonne++;
	}
}

void completer_bloc_P5(MCUs* bloc, uint32_t largeur, uint32_t hauteur) {
	/*Complétion en hauteur*/
	if (bloc->hauteur != hauteur) {
		for (size_t i = hauteur; i < bloc->hauteur; i++) {
			for (size_t j = 0; j < bloc->largeur; j++) {
				bloc->Y[i][j] = bloc->Y[hauteur - 1][j];
			}
		}
	}

	/*Complétion en largeur*/
	if (bloc->largeur != largeur) {
		for (size_t i = 0; i < bloc->hauteur; i++) { //Modification ici (à vérifier)
			for (size_t j = largeur; j < bloc->largeur; j++) {
				bloc->Y[i][j] = bloc->Y[i][largeur - 1];
			}
		}
	}
}

void completer_bloc_P6(MCUs* bloc, uint32_t largeur, uint32_t hauteur) {
	/*Complétion en hauteur*/
	if (bloc->hauteur != hauteur) {
		for (size_t i = hauteur; i < bloc->hauteur; i++) {
			for (size_t j = 0; j < bloc->largeur; j++) {
				bloc->Y[i][j] = bloc->Y[hauteur - 1][j];
				bloc->Cb[i][j] = bloc->Cb[hauteur -1][j];
				bloc->Cr[i][j] = bloc->Cr[hauteur - 1][j];
			}
		}
	}

	/*Complétion en largeur*/
	if (bloc->largeur != largeur) {
		for (size_t i = 0; i < bloc->hauteur; i++) {
			for (size_t j = largeur; j < bloc->largeur; j++) {
				bloc->Y[i][j] = bloc->Y[i][largeur - 1];
				bloc->Cb[i][j] = bloc->Cb[i][largeur - 1];
				bloc->Cr[i][j] = bloc->Cr[i][largeur - 1];
			}
		}
	}

}

void parse_P5(FILE* fichier, image_ppm* image, MCUs* bloc, uint32_t largeur, uint32_t hauteur) {
	uint8_t octet;
	size_t erreur;
	fseek(fichier, (long) bloc->numero_ligne * image->largeur * bloc->hauteur + bloc->numero_colonne * bloc->largeur, SEEK_CUR); //On start au bloc courant
	for (size_t i = 0; i < hauteur; i++) {
		for (size_t j = 0; j < largeur; j++) {
			erreur = fread(&octet, sizeof(uint8_t), 1, fichier);
			if (erreur != 1) exit(EXIT_FAILURE);
			//*bloc->matrice[i][j].couleurs = octet;
			bloc->Y[i][j] = octet;
		}
		fseek(fichier, (long) image->largeur - largeur, SEEK_CUR);
	}
	fseek(fichier, - (long) image->largeur * hauteur, SEEK_CUR);
	fseek(fichier, - (long) (bloc->numero_ligne * image->largeur * bloc->hauteur + bloc->numero_colonne * bloc->largeur), SEEK_CUR); //On revient au début

	completer_bloc_P5(bloc, largeur, hauteur);
}

void parse_P6(FILE* fichier, image_ppm* image, MCUs* bloc, uint32_t largeur, uint32_t hauteur) {
	uint8_t r, g, b;
	size_t erreur;
	fseek(fichier, (long) bloc->numero_ligne * image->largeur * bloc->hauteur * 3 + bloc->numero_colonne * bloc->largeur * 3, SEEK_CUR); //On start au bloc courant
	for (size_t i = 0; i < hauteur; i++) {
		for (size_t j = 0; j < largeur; j++) {
			erreur = fread(&r, sizeof(uint8_t), 1, fichier);
			if (erreur != 1) exit(EXIT_FAILURE);
			erreur = fread(&g, sizeof(uint8_t), 1, fichier);
			if (erreur != 1) exit(EXIT_FAILURE);
			erreur = fread(&b, sizeof(uint8_t), 1, fichier);
			if (erreur != 1) exit(EXIT_FAILURE);
			conversion(bloc, i, j, r, g, b);
		}
		fseek(fichier, (long) (image->largeur - largeur) * 3, SEEK_CUR);
	}
	fseek(fichier, - (long) (3 * image->largeur * hauteur), SEEK_CUR);
	fseek(fichier, - (long) (bloc->numero_ligne * image->largeur * bloc->hauteur * 3 + bloc->numero_colonne * bloc->largeur * 3), SEEK_CUR); //On revient au début

	completer_bloc_P6(bloc, largeur, hauteur);
}


void afficher_image(image_ppm* image) {
	printf("INFORMATIONS SUR L'IMAGE\n");
	printf("Largeur réelle de l'image: %d\n", image->largeur);
	printf("Hauteur réelle de l'image: %d\n", image->hauteur);
	printf("Largeur totale de l'image avec complétion des MCUs: %d\n", image->largeur_totale);
	printf("Hauteur totale de l'image avec complétion des MCUs: %d\n", image->hauteur_totale);
	printf("Plage de couleurs: %d\n", image->nb_couleurs);
	printf("Nombre de MCUs: %d\n", image->nb_MCUs);
	printf("Format de l'image: %2s\n", image->format);
}

void afficher_MCUs(MCUs* bloc, bool choix) {
	if (choix) {
		for (size_t i = 0; i < bloc->hauteur; i++) {
			for (size_t j = 0; j < bloc->largeur; j++) {
				printf("%02hhX ", bloc->Y[i][j]);
			}
			printf("\n");
		}
	}
	else {
		for (size_t i = 0; i < bloc->hauteur; i++) {
			for (size_t j = 0; j < bloc->largeur; j++) {
				printf("%02hhX ", bloc->Y[i][j]);
				printf("%02hhX ", bloc->Cb[i][j]);
				printf("%02hhX ", bloc->Cr[i][j]);
			}
			printf("\n");
		}
	}
}
