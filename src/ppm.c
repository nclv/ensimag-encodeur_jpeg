#include "ppm.h"

#include <stdint.h>

#include "conversion.h"

static image_ppm* lire_p5(FILE* fichier_ppm, image_ppm* image, uint8_t octet) {
    for (size_t i = 0; i < image->hauteur; i++) {
        for (size_t j = 0; j < image->largeur; j++) {
            size_t couleur = fread(&octet, sizeof(uint8_t), 1, fichier_ppm);
            if (couleur != 1) return NULL;
            image->matrice[i][j].triplet_rgb[0] = octet;
            image->matrice[i][j].triplet_rgb[1] = octet;
            image->matrice[i][j].triplet_rgb[2] = octet;
            conversion(&image->matrice[i][j]);
        }
    }
    return image;
}

static image_ppm* lire_p6(FILE* fichier_ppm, image_ppm* image) {
    for (size_t i = 0; i < image->hauteur; i++) {
        for (size_t j = 0; j < image->largeur; j++) {
            size_t r = fread(&image->matrice[i][j].triplet_rgb[0], sizeof(uint8_t), 1, fichier_ppm);
            if (r != 1) return NULL;
            size_t g = fread(&image->matrice[i][j].triplet_rgb[1], sizeof(uint8_t), 1, fichier_ppm);
            if (g != 1) return NULL;
            size_t b = fread(&image->matrice[i][j].triplet_rgb[2], sizeof(uint8_t), 1, fichier_ppm);
            if (b != 1) return NULL;
            conversion(&image->matrice[i][j]);
        }
    }
    return image;
}

image_ppm* lire_ppm(char* nom_fichier) {
    /*Ouverture du fichier*/
    FILE* fichier_ppm = fopen(nom_fichier, "r");
    if (fichier_ppm == NULL) {
        printf("ERREUR : Pointeur NULL dans lire_ppm() / Nom de fichier invalide\n");
        return NULL;
    }

    image_ppm* image = malloc(sizeof *image);
    if (image == NULL) {
        return NULL;
    }

    /*Parsing en-tete*/
    int mode = fscanf(fichier_ppm, "%2s", image->mode);
    if (mode != 1) return NULL;
    int largeur = fscanf(fichier_ppm, "%d", &image->largeur);
    if (largeur != 1) return NULL;
    int hauteur = fscanf(fichier_ppm, "%d", &image->hauteur);
    if (hauteur != 1) return NULL;
    int plage = fscanf(fichier_ppm, "%d", &image->nb_couleurs);
    if (plage != 1) return NULL;

    /*
		Initialisation de la matrice
		Voir https://stackoverflow.com/a/15067120
	*/
    image->matrice = malloc(image->hauteur * sizeof *image->matrice);
    if (image->matrice == NULL) {
        free(image);
        return NULL;
    }

    image->matrice[0] = malloc(image->hauteur * image->largeur * sizeof image->matrice);
    if (image->matrice[0] == NULL) {
        free(image->matrice);
        free(image);
        return NULL;
    }

    for (size_t i = 0; i < image->hauteur; i++) {
        image->matrice[i] = image->matrice[0] + i * image->largeur;
    }

    /* Parsing \n (de valeur 10) */
    /* Lecture d'un élément de donnée de uint8_t byte de long */
    uint8_t octet;
    size_t newline = fread(&octet, sizeof(uint8_t), 1, fichier_ppm);
    if (newline != 1) return NULL;

    /*Remplissage de la matrice
	 *Remarque : on fait le choix de ne pas traiter les if dans une seule double boucle
	 *pour éviter les if à répétition
	 */
    if (!strcmp(image->mode, "P5")) {
        return lire_p5(fichier_ppm, image, octet);
    } else {
        return lire_p6(fichier_ppm, image);
    }

    fclose(fichier_ppm);

    return image;
}

void liberer_image(image_ppm** image) {
    free((*image)->matrice[0]);
	(*image)->matrice[0] = NULL;
    free((*image)->matrice);
	(*image)->matrice = NULL;
    free(*image);
	*image = NULL;
}

void afficher_image(image_ppm* image) {
    printf("Mode %s\n", image->mode);
    printf("Largeur: %d\n", image->largeur);
    printf("Hauteur: %d\n", image->hauteur);
    printf("Plage d'une composante de couleurs: %d\n", image->nb_couleurs);
    printf("Affichage des pixels en rgb\n");
    for (uint32_t i = 0; i < image->hauteur; i++) {
        for (uint32_t j = 0; j < image->largeur; j++) {
            printf("%i %i %i\n", image->matrice[i][j].triplet_rgb[0],
                   image->matrice[i][j].triplet_rgb[1],
                   image->matrice[i][j].triplet_rgb[2]);
        }
    }
    printf("Affichage des pixels en ycbcr\n");
    for (uint32_t i = 0; i < image->hauteur; i++) {
        for (uint32_t j = 0; j < image->largeur; j++) {
            printf("%i %i %i\n", image->matrice[i][j].triplet_ycbcr[0],
                   image->matrice[i][j].triplet_ycbcr[1],
                   image->matrice[i][j].triplet_ycbcr[2]);
        }
    }
}
