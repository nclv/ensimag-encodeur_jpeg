#include "ppm.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

FILE* ouvrir_fichier(char* nom_fichier) {
    /*Ouverture du fichier*/
    FILE* fichier = fopen(nom_fichier, "r");
    if (fichier == NULL) exit(EXIT_FAILURE);

    return fichier;
}

void fermer_fichier(FILE* fichier) {
    /*Fermeture du fichier*/
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

bool choisir_format(char format[]) {
    /*Booléen sur le format*/
    return !strcmp(format, NIVEAUX_GRIS);
}

static void allocate_MCUs_grayscale(MCUs *bloc) {
    bloc->largeur = TAILLE_BLOC;
    bloc->hauteur = TAILLE_BLOC;

    bloc->Cb = NULL;
    bloc->Cr = NULL;
    bloc->comp_Y = NULL;
    bloc->comp_Cb = NULL;
    bloc->comp_Cr = NULL;

    /*Allocation Y*/
    bloc->Y = malloc(bloc->hauteur * sizeof(uint8_t*));
    if (bloc->Y == NULL) {
        free(bloc);
        exit(EXIT_FAILURE);
    }
    for (size_t i = 0; i < bloc->hauteur; i++) {
        bloc->Y[i] = malloc(bloc->largeur * sizeof(uint8_t));
    }
}

MCUs* initialiser_MCUs(image_ppm* image, uint8_t facteurs[NOMBRE_FACTEURS]) {
    /*Initialisation des blocs*/
    MCUs* bloc = malloc(sizeof(MCUs));
    if (bloc == NULL) exit(EXIT_FAILURE);

    /*On traite les boucles dans les if
	 *c'est plus performant que de tout traiter en
	 *même temps. On évite les if dans les boucles*/
    /*Grayscale*/
    if (choisir_format(image->format)) {
        allocate_MCUs_grayscale(bloc);
    } else { /*RGB*/
        bloc->largeur = (uint32_t)facteurs[H1] * TAILLE_BLOC;
        bloc->hauteur = (uint32_t)facteurs[V1] * TAILLE_BLOC;
        /*Sous-échantillonage simple*/
        if (facteurs[H2] == 1 && facteurs[V2] == 1) {
            bloc->comp_Cb = NULL;
        }
        if (facteurs[H3] == 1 && facteurs[V3] == 1) {
            bloc->comp_Cr = NULL;
        }
        /*1x1, 1x1, 1x1*/
        if (bloc->largeur == TAILLE_BLOC && bloc->hauteur == TAILLE_BLOC) {
            bloc->comp_Y = NULL;
        }

        /*Allocations Y, Cb, Cr*/
        bloc->Y = malloc(bloc->hauteur * sizeof(uint8_t*));
        bloc->Cb = malloc(bloc->hauteur * sizeof(uint8_t*));
        bloc->Cr = malloc(bloc->hauteur * sizeof(uint8_t*));
        for (size_t i = 0; i < bloc->hauteur; i++) {
            bloc->Y[i] = malloc(bloc->largeur * sizeof(uint8_t));
            bloc->Cb[i] = malloc(bloc->largeur * sizeof(uint8_t));
            bloc->Cr[i] = malloc(bloc->largeur * sizeof(uint8_t));
        }
    }

    /*Attribution des paramètres*/
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
	 *des MCUs*/
    if (largeur % bloc->largeur != 0) {
        horizontale = (uint32_t)horizontale + 1;
    }

    /*Nombre total de colonnes*/
    return horizontale * bloc->largeur;
}

uint32_t recuperer_hauteur(MCUs* bloc, uint32_t hauteur) {
    uint32_t verticale = hauteur / bloc->hauteur;

    /*Cas où la hauteur n'est pas un multiple de la hauteur
	 *des MCUs*/
    if (hauteur % bloc->hauteur != 0) {
        verticale = (uint32_t)verticale + 1;
    }

    /*Nombre total de lignes*/
    return verticale * bloc->hauteur;
}

void recuperer_MCUs(FILE* fichier, image_ppm* image, MCUs* bloc) {
    uint32_t largeur, hauteur;

    /*Arrangements sur hauteur et largeur*/
    if (bloc->numero_colonne == (image->largeur_totale / bloc->largeur) - 1) {
        largeur = bloc->largeur - (image->largeur_totale - image->largeur);
    } else {
        largeur = bloc->largeur;
    }
    if (bloc->numero_ligne == (image->hauteur_totale / bloc->hauteur) - 1) {
        hauteur = bloc->hauteur - (image->hauteur_totale - image->hauteur);
    } else {
        hauteur = bloc->hauteur;
    }

    /*Parsing des formats*/
    if (choisir_format(image->format)) {
        parse_grayscale(fichier, image, bloc, largeur, hauteur);
    } else {
        parse_RGB(fichier, image, bloc, largeur, hauteur);
    }

    /*Bloc suivant*/
    if (bloc->numero_colonne == (image->largeur_totale / bloc->largeur) - 1) {
        bloc->numero_ligne++;
        bloc->numero_colonne = 0;
    } else {
        bloc->numero_colonne++;
    }
}

void completer_grayscale(MCUs* bloc, uint32_t largeur, uint32_t hauteur) {
    /*Complétion eh hauteur*/
    if (bloc->hauteur != hauteur) {
        for (size_t i = hauteur; i < bloc->hauteur; i++) {
            for (size_t j = 0; j < bloc->largeur; j++) {
                bloc->Y[i][j] = bloc->Y[hauteur - 1][j];
            }
        }
    }

    /*Complétion en largeur*/
    if (bloc->largeur != largeur) {
        for (size_t i = 0; i < bloc->hauteur; i++) {
            for (size_t j = largeur; j < bloc->largeur; j++) {
                bloc->Y[i][j] = bloc->Y[i][largeur - 1];
            }
        }
    }
}

void completer_RGB(MCUs* bloc, uint32_t largeur, uint32_t hauteur) {
    /*Complétion en hauteur*/
    if (bloc->hauteur != hauteur) {
        for (size_t i = hauteur; i < bloc->hauteur; i++) {
            for (size_t j = 0; j < bloc->largeur; j++) {
                bloc->Y[i][j] = bloc->Y[hauteur - 1][j];
                bloc->Cb[i][j] = bloc->Cb[hauteur - 1][j];
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

void parse_grayscale(FILE* fichier, image_ppm* image, MCUs* bloc, uint32_t largeur, uint32_t hauteur) {
    uint8_t octet;
    size_t erreur;
    /*Placement au début de bloc*/
    fseek(fichier, (long)bloc->numero_ligne * image->largeur * bloc->hauteur + bloc->numero_colonne * bloc->largeur, SEEK_CUR);
    for (size_t i = 0; i < hauteur; i++) {
        for (size_t j = 0; j < largeur; j++) {
            erreur = fread(&octet, sizeof(uint8_t), 1, fichier);
            if (erreur != 1) {
                printf("ERREUR\n");
                exit(EXIT_FAILURE);
            }
            bloc->Y[i][j] = octet;
        }
        /*Saut prochaine ligne du bloc*/
        fseek(fichier, (long)image->largeur - largeur, SEEK_CUR);
    }
    /*On remonte au début du bloc*/
    fseek(fichier, -(long)image->largeur * hauteur, SEEK_CUR);
    /*On remonte au début du flux d'octets de l'image*/
    fseek(fichier, -(long)(bloc->numero_ligne * image->largeur * bloc->hauteur + bloc->numero_colonne * bloc->largeur), SEEK_CUR);

    /*On complète le bloc*/
    completer_grayscale(bloc, largeur, hauteur);
}

void parse_RGB(FILE* fichier, image_ppm* image, MCUs* bloc, uint32_t largeur, uint32_t hauteur) {
    uint8_t r, g, b;
    size_t erreur;
    /*Placement au début du bloc*/
    fseek(fichier, (long)bloc->numero_ligne * image->largeur * bloc->hauteur * 3 + bloc->numero_colonne * bloc->largeur * 3, SEEK_CUR);
    for (size_t i = 0; i < hauteur; i++) {
        for (size_t j = 0; j < largeur; j++) {
            erreur = fread(&r, sizeof(uint8_t), 1, fichier);
            if (erreur != 1) exit(EXIT_FAILURE);
            erreur = fread(&g, sizeof(uint8_t), 1, fichier);
            if (erreur != 1) exit(EXIT_FAILURE);
            erreur = fread(&b, sizeof(uint8_t), 1, fichier);
            if (erreur != 1) exit(EXIT_FAILURE);
            // conversion(bloc, i, j, r, g, b);
            bloc->Y[i][j] = (uint8_t)(0.299 * r + 0.587 * g + 0.114 * b);
            bloc->Cb[i][j] = (uint8_t)(-0.1687 * r - 0.3313 * g + 0.5000 * b + 128);
            bloc->Cr[i][j] = (uint8_t)(0.5000 * r - 0.4187 * g - 0.0813 * b + 128);
        }
        /*Saut prochaine ligne du bloc*/
        fseek(fichier, (long)(image->largeur - largeur) * 3, SEEK_CUR);
    }
    /*On remonte au début du bloc*/
    fseek(fichier, -(long)(3 * image->largeur * hauteur), SEEK_CUR);
    /*On remonte au début du flux d'octets de l'image*/
    fseek(fichier, -(long)(bloc->numero_ligne * image->largeur * bloc->hauteur * 3 + bloc->numero_colonne * bloc->largeur * 3), SEEK_CUR);

    /*On complète le bloc*/
    completer_RGB(bloc, largeur, hauteur);
}

void afficher_MCUs(image_ppm* image, MCUs* bloc) {
    /*Grayscale*/
    if (choisir_format(image->format)) {
        for (size_t i = 0; i < bloc->hauteur; i++) {
            for (size_t j = 0; j < bloc->largeur; j++) {
                printf("%02hhX ", bloc->Y[i][j]);
            }
            printf("\n");
        }
    } else { /*RGB*/
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
