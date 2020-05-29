#include "ppm.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* type: char[]
 * rtype: bool
 * Choisit le mode de remplissage P5 ou P6
 */
static bool is_grayscale_format(const char format[3]) {
    /*Booléen sur le format*/
    return !strcmp(format, "P5");
}

void free_mcu(MCUs **mcu) {
    for (size_t i = 0; i < (*mcu)->hauteur; i++) {
        free((*mcu)->Y[i]);
    }
    free((*mcu)->Y);
    (*mcu)->Y = NULL;
    if ((*mcu)->Cb) {
        for (size_t i = 0; i < (*mcu)->hauteur; i++) {
            free((*mcu)->Cb[i]);
        }
        free((*mcu)->Cb);
        (*mcu)->Cb = NULL;
    }
    if ((*mcu)->Cr) {
        for (size_t i = 0; i < (*mcu)->hauteur; i++) {
            free((*mcu)->Cr[i]);
        }
        free((*mcu)->Cr);
        (*mcu)->Cr = NULL;
    }

    free(*mcu);
    *mcu = NULL;
}

/*
 * type: FILE*
 * rtype: image_ppm*
 * Réalise le parsing du fichier passé en paramètre pour en
 * extraire une image ppm
 */
image_ppm* parse_entete(FILE* fichier) {
    /*Initialisation de l'image*/
    image_ppm* image = malloc(sizeof(image_ppm));
    if (image == NULL) exit(EXIT_FAILURE);

    /*Parsing en-tête*/
    char format_image_ppm[3];

    int erreur_entete;
    erreur_entete = fscanf(fichier, "%2s", format_image_ppm);
    image->nb_components = is_grayscale_format(format_image_ppm) ? 1 : 3;
    if (erreur_entete != 1) exit(EXIT_FAILURE);
    erreur_entete = fscanf(fichier, "%u", &image->largeur);
    if (erreur_entete != 1) exit(EXIT_FAILURE);
    erreur_entete = fscanf(fichier, "%u", &image->hauteur);
    if (erreur_entete != 1) exit(EXIT_FAILURE);
    erreur_entete = fscanf(fichier, "%u", &image->plage_couleurs);
    if (erreur_entete != 1) exit(EXIT_FAILURE);

    /* Initialisation */
    image->hauteur_totale = 0;
    image->largeur_totale = 0;
    image->nb_components = 0;
    image->nb_MCUs = 0;

    /*Parsing \n (de valeur 10)*/
    uint8_t octet;
    size_t erreur_backslash = fread(&octet, sizeof(uint8_t), 1, fichier);
    if (erreur_backslash != 1) exit(EXIT_FAILURE);

    return image;
}

/* type: MCUs*
 * rtype: void
 * Alloue l'espace nécessaire pour une image en
 * niveaux de gris seulement
 */
static void allocate_MCUs_grayscale(MCUs* mcu) {
    mcu->largeur = TAILLE_DATA_UNIT;
    mcu->hauteur = TAILLE_DATA_UNIT;

    mcu->Cb = NULL;
    mcu->Cr = NULL;

    /*Allocation Y*/
    mcu->Y = malloc(64);  // mcu->hauteur * sizeof(int16_t*)
    if (mcu->Y == NULL) {
        free(mcu);
        exit(EXIT_FAILURE);
    }
    for (size_t i = 0; i < mcu->hauteur; i++) {
        mcu->Y[i] = malloc(64);  // mcu->largeur * sizeof(int16_t)
    }
}

/* type: MCUs*
 * rtype: void
 * Alloue l'espace nécessaire pour une image en
 * couleurs RGB
 */
static void allocate_MCUs_RGB(MCUs* mcu, uint8_t sampling_factors[NB_COLOR_COMPONENTS][NB_DIRECTIONS]) {
    mcu->largeur = (uint32_t)sampling_factors[Y][H] * TAILLE_DATA_UNIT;
    mcu->hauteur = (uint32_t)sampling_factors[Y][V] * TAILLE_DATA_UNIT;

    /*Allocations Y, Cb, Cr*/
    mcu->Y = malloc(mcu->hauteur * sizeof(int16_t*));
    mcu->Cb = malloc(mcu->hauteur * sizeof(int16_t*));
    mcu->Cr = malloc(mcu->hauteur * sizeof(int16_t*));
    for (size_t i = 0; i < mcu->hauteur; i++) {
        mcu->Y[i] = malloc(mcu->largeur * sizeof(int16_t));
        mcu->Cb[i] = malloc(mcu->largeur * sizeof(int16_t));
        mcu->Cr[i] = malloc(mcu->largeur * sizeof(int16_t));
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

/* type: image_ppm*, uint8_t
 * rtype: MCUs*
 * Initialise les MCUs et les attributs de l'image
 */
MCUs* initialiser_MCUs(image_ppm* image, uint8_t sampling_factors[NB_COLOR_COMPONENTS][NB_DIRECTIONS]) {
    /*Initialisation des blocs*/
    MCUs* mcu = malloc(sizeof(MCUs));
    if (mcu == NULL) exit(EXIT_FAILURE);

    (image->nb_components == 1) ? allocate_MCUs_grayscale(mcu) : allocate_MCUs_RGB(mcu, sampling_factors);

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
            mcu->Y[i][j] = (int16_t)octet;
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

            /* Conversion RGB -> YCbCr */
            mcu->Y[i][j] = (int16_t)(0.299 * r + 0.587 * g + 0.114 * b);
            mcu->Cb[i][j] = (int16_t)(-0.1687 * r - 0.3313 * g + 0.5000 * b + 128);
            mcu->Cr[i][j] = (int16_t)(0.5000 * r - 0.4187 * g - 0.0813 * b + 128);
        }
        /*Saut prochaine ligne du mcu*/
        fseek(fichier, (long)(image->largeur - largeur) * 3, SEEK_CUR);
    }
    /*On remonte au début du mcu*/
    fseek(fichier, -(long)(3 * image->largeur * hauteur), SEEK_CUR);
    /*On remonte au début du flux d'octets de l'image*/
    fseek(fichier, -(long)(mcu->numero_ligne * image->largeur * mcu->hauteur * 3 + mcu->numero_colonne * mcu->largeur * 3), SEEK_CUR);
}

/* type: FILE*, image_ppm*, MCUs*
 * rtype: void
 * Récupère les MCU et les formate
 */
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
    if (image->nb_components == 1) {
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

static void afficher_MCU_grayscale(const MCUs* mcu) {
    for (size_t i = 0; i < mcu->hauteur; i++) {
        for (size_t j = 0; j < mcu->largeur; j++) {
            printf("%02" PRIx16 " ", (uint16_t)mcu->Y[i][j]);
        }
        printf("\n");
    }
}

static void afficher_MCU_RGB(const MCUs* mcu) {
    for (size_t i = 0; i < mcu->hauteur; i++) {
        for (size_t j = 0; j < mcu->largeur; j++) {
            printf("%02" PRIx16 "", (uint16_t)mcu->Y[i][j]);
            printf("%02" PRIx16 "", (uint16_t)mcu->Cb[i][j]);
            printf("%02" PRIx16 " ", (uint16_t)mcu->Cr[i][j]);
        }
        printf("\n");
    }
}

void afficher_MCUs(uint8_t nb_components, const MCUs* mcu) {
    printf("\nAffichage du MCU: \n");
    if (nb_components == 1) {
        afficher_MCU_grayscale(mcu);
    } else {
        afficher_MCU_RGB(mcu);
    }
}

void afficher_image(const image_ppm *image) {
    printf("Largeur image : %u\n", image->largeur);
    printf("Hauteur image : %u\n", image->hauteur);
    printf("Largeur totale : %u\n", image->largeur_totale);
    printf("Hauteur totale : %u\n", image->hauteur_totale);
    printf("Plage de couleurs : %u\n", image->plage_couleurs);
    printf("Nombre de MCUs : %u\n", image->nb_MCUs);
    printf("Nombre de composantes : %d\n", image->nb_components);
}