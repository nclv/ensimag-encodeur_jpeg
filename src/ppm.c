#include "ppm.h"
#include "conversion.h"

extern pixel** initialiser_matrice(uint32_t hauteur, uint32_t largeur, uint32_t nb_couleurs) {
	/*Initialisation en hauteur*/
	pixel** matrice = malloc(hauteur * sizeof(pixel*));
	if (matrice == NULL) exit(EXIT_FAILURE);

	/*Initialisation en largeur*/
	for (size_t i = 0; i < hauteur; i++) {
		matrice[i] = malloc(largeur * sizeof(pixel));	
		if (matrice[i] == NULL)  exit(EXIT_FAILURE);
	}

	/*Initialisation des couleurs
	 *1 pour P5
	 *3 pour P6
	 */
	for (size_t i = 0; i < hauteur; i++) {
		for(size_t j = 0; j < largeur; j++) {
			matrice[i][j].couleurs = malloc(nb_couleurs * sizeof(uint8_t));
			if (matrice[i][j].couleurs == NULL) exit(EXIT_FAILURE);
		}
	}

	return matrice;
}

extern bool choix_remplissage(char mode[]) {
	return (!strcmp(mode, NIVEAUX_GRIS));
}

extern void parse_p5(image_ppm* image, FILE* fichier) {
	uint8_t octet;
	size_t erreur;
	/*Lecture des pixels*/
	for (size_t i = 0; i < image->hauteur; i++) {
		for (size_t j = 0; j < image->largeur; j++) {
			erreur = fread(&octet, sizeof(uint8_t), 1, fichier);
			if (erreur != 1) exit(EXIT_FAILURE);
			*image->matrice[i][j].couleurs = octet;
		}
	}
	fclose(fichier);
}

extern void parse_p6(image_ppm* image, FILE* fichier) {
	size_t erreur;
	uint8_t r, g, b;
	/*Lecture des pixels*/
	for (size_t i = 0; i < image->hauteur; i++) {
		for (size_t j = 0; j < image->largeur; j++) {
			erreur = fread(&r, sizeof(uint8_t), 1, fichier);
			if (erreur != 1) exit(EXIT_FAILURE);
			erreur = fread(&g, sizeof(uint8_t), 1, fichier);
			if (erreur != 1) exit(EXIT_FAILURE);
			erreur = fread(&b, sizeof(uint8_t), 1, fichier);
			if (erreur != 1) exit(EXIT_FAILURE);
			conversion(&image->matrice[i][j], r, g, b);
		}
	}
	fclose(fichier);
}

extern image_ppm* lire_ppm(char* nom_fichier, uint8_t h, uint8_t v) {
	/*Ouverture du fichier*/
	FILE* fichier_ppm = fopen(nom_fichier, "r");
	if (fichier_ppm == NULL) exit(EXIT_FAILURE);

	/*Initialisation de l'image*/
	image_ppm* image = malloc(sizeof(image_ppm));
	if (image == NULL) exit(EXIT_FAILURE);

	/*Parsing en-tête*/
	int mode = fscanf(fichier_ppm, "%2s", image->mode);
    if (mode != 1) exit(EXIT_FAILURE);
	int largeur = fscanf(fichier_ppm, "%d", &image->largeur);
	if (largeur != 1) exit(EXIT_FAILURE);
	int hauteur = fscanf(fichier_ppm, "%d", &image->hauteur);
	if (hauteur != 1) exit(EXIT_FAILURE);
	int plage = fscanf(fichier_ppm, "%d", &image->nb_couleurs);
	if (plage != 1) exit(EXIT_FAILURE);

	/*Choix du sous-échantillonage*/
	ech echantillonage = recuperer_echantillonage(h, v);
	MCUs* bloc = recuperer_dimensions(echantillonage);
	uint32_t hauteur_totale = recuperer_hauteur(bloc, image->hauteur);
	uint32_t largeur_totale = recuperer_largeur(bloc, image->largeur);

	/*Initialisation de la matrice
	 *avec complétion des MCUs*/
	image->matrice = initialiser_matrice(hauteur_totale, largeur_totale, image->nb_couleurs);
	
	/*Parsing \n (de valeur 10) */
	uint8_t octet;
	size_t erreur = fread(&octet, sizeof(uint8_t), 1, fichier_ppm);
	if (erreur != 1) exit(EXIT_FAILURE);

	/*Remplissage de la matrice avec conversion en YCbCr
	 *Complétion de la matrice en fonction de la taille des MCUs
	 *Remarque : on fait le choix de ne pas traiter les if dans une seule double boucle
	 *pour éviter les if à répétition
	 */
	if (choix_remplissage(image->mode)) {
		parse_p5(image, fichier_ppm);
		completer_image_p5(image, hauteur_totale, largeur_totale);
	}
	else {
		parse_p6(image, fichier_ppm);
		completer_image_p6(image, hauteur_totale, largeur_totale);
	}
	
	return image;
}

extern void completer_image_p5(image_ppm* image, uint32_t hauteur, uint32_t largeur) {
	/*Complétion en hauteur*/
	for (size_t i = image->hauteur; i < hauteur; i++) {
		for (size_t j = 0; j < image->largeur; j++) {
			*image->matrice[i][j].couleurs = *image->matrice[image->hauteur - 1][j].couleurs;	
		}
	}

	/*Complétion en largeur*/
	for (size_t i = 0; i < hauteur; i++) {
		for (size_t j = image->largeur; j < largeur; j++) {
			*image->matrice[i][j].couleurs = *image->matrice[i][image->largeur - 1].couleurs;
		}
	}

	image->hauteur = hauteur;
	image->largeur = largeur;

}

extern void completer_image_p6(image_ppm* image, uint32_t hauteur, uint32_t largeur) {
	/*Complétion en hauteur*/
	for (size_t i = image->hauteur; i < hauteur; i++) {
		for (size_t j = 0; j < largeur; j++) {
			image->matrice[i][j].couleurs[Y] = image->matrice[image->hauteur - 1][j].couleurs[Y];
			image->matrice[i][j].couleurs[CB] = image->matrice[image->hauteur - 1][j].couleurs[CB];
			image->matrice[i][j].couleurs[CR] = image->matrice[image->hauteur - 1][j].couleurs[CR];
		}
	}
	
	/*Complétion en largeur*/
	for (size_t i = 0; i < hauteur; i++) {
		for (size_t j = image->largeur; j < largeur; j++) {
			image->matrice[i][j].couleurs[Y] = image->matrice[i][image->largeur - 1].couleurs[Y];
			image->matrice[i][j].couleurs[CB] = image->matrice[i][image->largeur - 1].couleurs[CB];
			image->matrice[i][j].couleurs[CR] = image->matrice[i][image->largeur - 1].couleurs[CR];
		}
	}

	image->hauteur = hauteur;
	image->largeur = largeur;

}

