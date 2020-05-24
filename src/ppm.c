#include "ppm.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

/* type: char
 * rtype: bool
 * Choisit le mode de remplissage P5 ou P6
 */
static bool is_grayscale_format(char format[]) {
    /*Booléen sur le format*/
    return !strcmp(format, NIVEAUX_GRIS);
}

static void allocate_MCUs_grayscale(MCUs* bloc) {
    bloc->largeur = TAILLE_BLOC;
    bloc->hauteur = TAILLE_BLOC;

    bloc->Cb = NULL;
    bloc->Cr = NULL;
    // bloc->comp_Y = NULL;
    // bloc->comp_Cb = NULL;
    // bloc->comp_Cr = NULL;

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

static void allocate_MCUs_RGB(MCUs* bloc, uint8_t facteurs[NOMBRE_FACTEURS]) {
    bloc->largeur = (uint32_t)facteurs[H1] * TAILLE_BLOC;
    bloc->hauteur = (uint32_t)facteurs[V1] * TAILLE_BLOC;
    /*Sous-échantillonage simple*/
    // if (facteurs[H2] == 1 && facteurs[V2] == 1) {
    //     bloc->comp_Cb = NULL;
    // }
    // if (facteurs[H3] == 1 && facteurs[V3] == 1) {
    //     bloc->comp_Cr = NULL;
    // }
    // /*1x1, 1x1, 1x1*/
    // if (bloc->largeur == TAILLE_BLOC && bloc->hauteur == TAILLE_BLOC) {
    //     bloc->comp_Y = NULL;
    // }

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

/* type: MCUs*
 * type: uint32_t
 * rtype: uint32_t
 * Récupère la largeur des MCUs
 */
static uint32_t recuperer_largeur(MCUs* mcu, uint32_t largeur) {
    uint32_t nb_mcu_horizontale = largeur / mcu->largeur;

    /*Cas où la largeur n'est pas un multiple de la largeur
	 *des MCUs*/
    if (largeur % mcu->largeur != 0) {
        nb_mcu_horizontale = (uint32_t)nb_mcu_horizontale + 1;
    }

    /*Nombre total de colonnes*/
    return nb_mcu_horizontale * mcu->largeur;
}

/* type: MCUs*
 * type: uint32_t
 * rtype: uint32_t
 * Récupère la hauteur des MCUs
 */
static uint32_t recuperer_hauteur(MCUs* mcu, uint32_t hauteur) {
    uint32_t nb_mcu_verticale = hauteur / mcu->hauteur;

    /*Cas où la hauteur n'est pas un multiple de la hauteur
	 *des MCUs*/
    if (hauteur % mcu->hauteur != 0) {
        nb_mcu_verticale = (uint32_t)nb_mcu_verticale + 1;
    }

    /*Nombre total de lignes*/
    return nb_mcu_verticale * mcu->hauteur;
}

MCUs* initialiser_MCUs(image_ppm* image, uint8_t facteurs[NOMBRE_FACTEURS]) {
    /*Initialisation des blocs*/
    MCUs* mcu = malloc(sizeof(MCUs));
    if (mcu == NULL) exit(EXIT_FAILURE);

    is_grayscale_format(image->format) ? allocate_MCUs_grayscale(mcu) : allocate_MCUs_RGB(mcu, facteurs);

    /*Attribution des paramètres*/
    mcu->numero_ligne = 0;
    mcu->numero_colonne = 0;
    image->largeur_totale = recuperer_largeur(mcu, image->largeur);
    image->hauteur_totale = recuperer_hauteur(mcu, image->hauteur);
    image->nb_MCUs = (image->largeur_totale * image->hauteur_totale) / (mcu->largeur * mcu->hauteur);

    return mcu;
}

/* type: MCUs*
 * type: uint32_t
 * type: uint32_t
 * rtype: void
 * Complète en largeur et en hauteur le MCU donné
 * Pour un format P5
 */
static void completer_grayscale(MCUs* mcu, uint32_t largeur, uint32_t hauteur) {
    /*Complétion eh hauteur*/
    if (mcu->hauteur != hauteur) {
        for (size_t i = hauteur; i < mcu->hauteur; i++) {
            for (size_t j = 0; j < mcu->largeur; j++) {
                mcu->Y[i][j] = mcu->Y[hauteur - 1][j];
            }
        }
    }

    /*Complétion en largeur*/
    if (mcu->largeur != largeur) {
        for (size_t i = 0; i < mcu->hauteur; i++) {
            for (size_t j = largeur; j < mcu->largeur; j++) {
                mcu->Y[i][j] = mcu->Y[i][largeur - 1];
            }
        }
    }
}

/* type: FILE*
 * type: image_ppm*
 * type: MCUs*
 * type: uint32_t
 * type: uint32_t
 * rtype: void
 * Parse l'image en P5 (MCU par MCU à la volée)
 */
static void completer_RGB(MCUs* mcu, uint32_t largeur, uint32_t hauteur) {
    /*Complétion en hauteur*/
    if (mcu->hauteur != hauteur) {
        for (size_t i = hauteur; i < mcu->hauteur; i++) {
            for (size_t j = 0; j < mcu->largeur; j++) {
                mcu->Y[i][j] = mcu->Y[hauteur - 1][j];
                mcu->Cb[i][j] = mcu->Cb[hauteur - 1][j];
                mcu->Cr[i][j] = mcu->Cr[hauteur - 1][j];
            }
        }
    }

    /*Complétion en largeur*/
    if (mcu->largeur != largeur) {
        for (size_t i = 0; i < mcu->hauteur; i++) {
            for (size_t j = largeur; j < mcu->largeur; j++) {
                mcu->Y[i][j] = mcu->Y[i][largeur - 1];
                mcu->Cb[i][j] = mcu->Cb[i][largeur - 1];
                mcu->Cr[i][j] = mcu->Cr[i][largeur - 1];
            }
        }
    }
}

/* type: FILE*
 * type: image_ppm*
 * type: MCUs*
 * type: uint32_t
 * type: uint32_t
 * rtype: void
 * Parse l'image en P5 (MCU par MCU à la volée)
 */
static void parse_grayscale(FILE* fichier, image_ppm* image, MCUs* mcu, uint32_t largeur, uint32_t hauteur) {
    uint8_t octet;
    size_t erreur;
    /*Placement au début de mcu*/
    fseek(fichier, (long)mcu->numero_ligne * image->largeur * mcu->hauteur + mcu->numero_colonne * mcu->largeur, SEEK_CUR);
    for (size_t i = 0; i < hauteur; i++) {
        for (size_t j = 0; j < largeur; j++) {
            erreur = fread(&octet, sizeof(uint8_t), 1, fichier);
            if (erreur != 1) {
                printf("ERREUR\n");
                exit(EXIT_FAILURE);
            }
            mcu->Y[i][j] = octet;
        }
        /*Saut prochaine ligne du mcu*/
        fseek(fichier, (long)image->largeur - largeur, SEEK_CUR);
    }
    /*On remonte au début du mcu*/
    fseek(fichier, -(long)image->largeur * hauteur, SEEK_CUR);
    /*On remonte au début du flux d'octets de l'image*/
    fseek(fichier, -(long)(mcu->numero_ligne * image->largeur * mcu->hauteur + mcu->numero_colonne * mcu->largeur), SEEK_CUR);
}

/* type: FILE*
 * type: image_ppm*
 * type: MCUs*
 * type: uint32_t
 * type: uint32_t
 * rtype: void
 * Parse l'image en P6 (MCU par MCU à la volée)
 */
static void parse_RGB(FILE* fichier, image_ppm* image, MCUs* mcu, uint32_t largeur, uint32_t hauteur) {
    uint8_t r, g, b;
    size_t erreur;
    /*Placement au début du mcu*/
    fseek(fichier, (long)mcu->numero_ligne * image->largeur * mcu->hauteur * 3 + mcu->numero_colonne * mcu->largeur * 3, SEEK_CUR);
    for (size_t i = 0; i < hauteur; i++) {
        for (size_t j = 0; j < largeur; j++) {
            erreur = fread(&r, sizeof(uint8_t), 1, fichier);
            if (erreur != 1) exit(EXIT_FAILURE);
            erreur = fread(&g, sizeof(uint8_t), 1, fichier);
            if (erreur != 1) exit(EXIT_FAILURE);
            erreur = fread(&b, sizeof(uint8_t), 1, fichier);
            if (erreur != 1) exit(EXIT_FAILURE);
            // conversion(mcu, i, j, r, g, b);
            mcu->Y[i][j] = (uint8_t)(0.299 * r + 0.587 * g + 0.114 * b);
            mcu->Cb[i][j] = (uint8_t)(-0.1687 * r - 0.3313 * g + 0.5000 * b + 128);
            mcu->Cr[i][j] = (uint8_t)(0.5000 * r - 0.4187 * g - 0.0813 * b + 128);
        }
        /*Saut prochaine ligne du mcu*/
        fseek(fichier, (long)(image->largeur - largeur) * 3, SEEK_CUR);
    }
    /*On remonte au début du mcu*/
    fseek(fichier, -(long)(3 * image->largeur * hauteur), SEEK_CUR);
    /*On remonte au début du flux d'octets de l'image*/
    fseek(fichier, -(long)(mcu->numero_ligne * image->largeur * mcu->hauteur * 3 + mcu->numero_colonne * mcu->largeur * 3), SEEK_CUR);
}

void recuperer_MCUs(FILE* fichier, image_ppm* image, MCUs* mcu) {
    uint32_t largeur, hauteur;

    /*Arrangements sur hauteur et largeur*/
    if (mcu->numero_colonne == (image->largeur_totale / mcu->largeur) - 1) {
        largeur = mcu->largeur - (image->largeur_totale - image->largeur);
    } else {
        largeur = mcu->largeur;
    }
    if (mcu->numero_ligne == (image->hauteur_totale / mcu->hauteur) - 1) {
        hauteur = mcu->hauteur - (image->hauteur_totale - image->hauteur);
    } else {
        hauteur = mcu->hauteur;
    }

    /*Parsing des formats*/
    if (is_grayscale_format(image->format)) {
        parse_grayscale(fichier, image, mcu, largeur, hauteur);
        /*On complète le mcu*/
        completer_grayscale(mcu, largeur, hauteur);
    } else {
        parse_RGB(fichier, image, mcu, largeur, hauteur);
        /*On complète le mcu*/
        completer_RGB(mcu, largeur, hauteur);
    }

    /*Mcu suivant*/
    if (mcu->numero_colonne == (image->largeur_totale / mcu->largeur) - 1) {
        mcu->numero_ligne++;
        mcu->numero_colonne = 0;
    } else {
        mcu->numero_colonne++;
    }
}

static void afficher_MCU_grayscale(MCUs* mcu) {
    for (size_t i = 0; i < mcu->hauteur; i++) {
        for (size_t j = 0; j < mcu->largeur; j++) {
            printf("%02hhX ", mcu->Y[i][j]);
        }
        printf("\n");
    }
}

static void afficher_MCU_RGB(MCUs* mcu) {
    for (size_t i = 0; i < mcu->hauteur; i++) {
        for (size_t j = 0; j < mcu->largeur; j++) {
            // printf("%02hhX ", mcu->Y[i][j]);
            printf("%02hhX ", mcu->Cb[i][j]);
            // printf("%02hhX ", mcu->Cr[i][j]);
        }
        printf("\n");
    }
}

void afficher_MCUs(char format[TAILLE_PPM], MCUs* mcu) {
    if (is_grayscale_format(format)) {
        afficher_MCU_grayscale(mcu);
    } else {
        afficher_MCU_RGB(mcu);
    }
}
