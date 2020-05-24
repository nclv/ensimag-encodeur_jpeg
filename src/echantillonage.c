#include "echantillonage.h"

/*bool verifier_facteurs(uint8_t h1, uint8_t h2, uint8_t h3, uint8_t v1, uint8_t v2, uint8_t v3) {
	return (h1 >= 1 && h1 <= 4 &&
			h2 >= 1 && h2 <= 4 &&
			h3 >= 1 && h3 <= 4 &&
			v1 >= 1 && v1 <= 4 &&
			v2 >= 1 && v2 <= 4 &&
			v3 >= 1 && v3 <= 4 &&
			(h1 * v1 + h2 * v2 + h3 * v3) <= 10 &&
			(h1 % h2 == 0) && (h1 % h3 == 0) &&
			(v1 % v2 == 0) && (v1 % v3 == 0);
}*/

ech recuperer_echantillonage(uint8_t h, uint8_t v) {
	/*Spécifiée uniquement pour les types du sujet
	 *pour le moment*/
	return (h == 4) ? SIMPLE : (v == 2) ? HORIZONTAL : VERTICAL_HORIZONTAL;
}

bool realiser_downsampling(ech echantillonage) {
	return echantillonage == HORIZONTAL || echantillonage == VERTICAL_HORIZONTAL;
}

void initialiser_composantes(MCUs* bloc) {
	bloc->nb_blocs_Y = (bloc->largeur * bloc->hauteur) / (TAILLE_BLOC * TAILLE_BLOC);

	bloc->comp_Y = malloc(sizeof(Y));

	for (size_t i = 0; i < bloc->nb_blocs_Y; i++) {
		bloc->comp_Y[i].Y_downsampling = malloc(TAILLE_BLOC * sizeof(uint8_t*));
		for (size_t j = 0; j < TAILLE_BLOC; j++) {
			bloc->comp_Y[i].Y_downsampling[j] = malloc(TAILLE_BLOC * sizeof(uint8_t));
		}
	}

	bloc->Cb_downsampling = malloc(TAILLE_BLOC * sizeof(uint8_t*));
	bloc->Cr_downsampling = malloc(TAILLE_BLOC * sizeof(uint8_t*));

	for (size_t i = 0; i < TAILLE_BLOC; i++) {
		bloc->Cb_downsampling[i] = malloc(TAILLE_BLOC * sizeof(uint8_t));
		bloc->Cr_downsampling[i] = malloc(TAILLE_BLOC * sizeof(uint8_t));
	}
}

size_t choix_indice(size_t i, size_t j) {
	/*On choisit de ne pas faire des instructions conditionnelles pour
	 *éviter de propager le calcul sur plusieurs cycles*/
	return 0 * (size_t) (i < TAILLE_BLOC && j < TAILLE_BLOC) +
		   1 * (size_t) (i < TAILLE_BLOC && j >= TAILLE_BLOC) +
		   2 * (size_t) (i >= TAILLE_BLOC && j < TAILLE_BLOC) +
		   3 * (size_t) (i >= TAILLE_BLOC && j >= TAILLE_BLOC);
}

void downsampling(MCUs* bloc) {
	/*Découpage des luminances*/
	for (size_t i = 0; i < bloc->hauteur; i++) {
		for (size_t j = 0; j < bloc->largeur; j++) {
			bloc->comp_Y[choix_indice(i, j)].Y_downsampling[i % TAILLE_BLOC][j % TAILLE_BLOC] = bloc->Y[i][j];
		}
	}

	/*Découpage et obtention des chrominances
	 *On fait le choix de traiter chaque cas
	 *dans des fonctions séparées pour éviter de
	 *tester par des instructions conditionnelles
	 *dans quel cas on se trouve
	 */
	switch(bloc->echantillonage) {
		case HORIZONTAL:
			compresser_horizontal(bloc);
			break;
		case VERTICAL_HORIZONTAL:
			compresser_vertical_horizontal(bloc);
			break;
		default:
			break;
	}
}

void compresser_horizontal(MCUs* bloc) {
	size_t k = 0;
	size_t l = 0;
	for (size_t i = 0; i < bloc->hauteur; i++) {
		for (size_t j = 0; j < bloc->largeur; j += 2) {
			/*Il faut respecter la division sur chaque composante
			 *Car deux uint8_t peuvent provoquer un dépassement
			 *Même en castant en int puis en recastant en uint8_t
			 *Note : il y a une légère perte d'informations*/
			bloc->Cb_downsampling[k][l] = (uint8_t)(bloc->Cb[i][j] / 2 + bloc->Cb[i][j + 1] / 2);
			bloc->Cr_downsampling[k][l] = (uint8_t)(bloc->Cr[i][j] / 2 + bloc->Cr[i][j + 1] / 2);
			l++;
		}
		k++;
		l = 0;
	}
}

void compresser_vertical_horizontal(MCUs* bloc) {
	size_t k = 0;
	size_t l = 0;
	for (size_t i = 0; i < bloc->hauteur; i += 2) {
		for (size_t j = 0; j < bloc->largeur; j += 2) {
			/*Il faut respecter la division sur chaque composante
			 *Car deux uint8_t peuvent provoquer un dépassement
			 *Même en castant en int puis en recastant en uint8_t
			 *Note: il y a une légère perte d'informations*/
			bloc->Cb_downsampling[k][l] = (uint8_t)(bloc->Cb[i][j] / 4
											+ bloc->Cb[i][j + 1] / 4
											+ bloc->Cb[i + 1][j] / 4
											+ bloc->Cb[i + 1][j + 1] / 4);
			bloc->Cr_downsampling[k][l] = (uint8_t)(bloc->Cr[i][j] / 4
											+ bloc->Cr[i][j + 1] / 4
											+ bloc->Cr[i + 1][j] / 4
											+ bloc->Cr[i + 1][j + 1] / 4);
			l++;
		}
		k++;
		l = 0;
	}
}

void afficher_MCUs_Downsampling(MCUs* bloc) {
	printf("Découpage de la luminance\n");
	for (size_t k = 0; k < bloc->nb_blocs_Y; k++) {
		printf("Composante luminance %ld\n", k + 1);
		for (size_t i = 0; i < TAILLE_BLOC; i++) {
			for (size_t j = 0; j < TAILLE_BLOC; j++) {
				printf("%02hhX ", bloc->comp_Y[k].Y_downsampling[i][j]);
			}
			printf("\n");
		}
	}

	printf("Chrominance Cb\n");
	for (size_t i = 0; i < TAILLE_BLOC; i++) {
		for (size_t j = 0; j < TAILLE_BLOC; j++) {
			printf("%02hhX ", bloc->Cb_downsampling[i][j]);
		}
		printf("\n");
	}

	printf("Chrominance Cr\n");
	for (size_t i = 0; i < TAILLE_BLOC; i++) {
		for (size_t j = 0; j < TAILLE_BLOC; j++) {
			printf("%02hhX ", bloc->Cr_downsampling[i][j]);
		}
		printf("\n");
	}
}
