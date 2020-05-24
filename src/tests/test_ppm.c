#include <stdio.h>
#include <stdlib.h>

#include "ppm.h"

void afficher_data_unit(uint8_t** data_unit) {
    printf("Data Unit\n");
    for (size_t i = 0; i < 8; i++) {
        for (size_t j = 0; j < 8; j++) {
            printf("%02hhX ", data_unit[i][j]);
        }
        printf("\n");
    }
}

void process_data_units(MCUs* mcu, uint8_t facteurs[NOMBRE_FACTEURS], uint8_t** data_unit) {
    /* Traitement des blocs Y */
    // printf("%d %d\n", facteurs[V1], facteurs[H1]);
    // for (size_t v = 0; v < facteurs[V1]; v++) {
    //     for (size_t h = 0; h < facteurs[H1]; h++) {
    //         for (size_t i = 0; i < 8; i++) {
    //             for (size_t j = 0; j < 8; j++) {
    //                 data_unit[i][j] = mcu->Y[i + 8 * v][j + 8 * h];
    //             }
    //         }
    //         /* Encodage de data_unit */
    //         afficher_data_unit(data_unit);
    //     }
    // }

    /* Traitement des blocs Cb avec échantillonnage horizontal */
    // printf("%d %d\n", facteurs[H2], facteurs[V2]);
    // for (size_t v = 0; v < facteurs[V1]; v++) {
    //     uint8_t count_horizontal = 1;
    //     for (size_t h = 0; h < facteurs[H1]; h++) {
    //         for (size_t i = 0; i < 8; i++) {
    //             for (size_t j = 0; j < 8; j++) {
    //                 data_unit[i][j] = (uint8_t)(data_unit[i][j] + mcu->Cb[i + 8 * v][j + 8 * h] * facteurs[H2] / facteurs[H1]);
    //             }
    //         }
    //         printf("Horizontal %d\n", count_horizontal);
    //         if (count_horizontal == facteurs[H1] / facteurs[H2]) {
    //             /* Encodage de data_unit */
    //             afficher_data_unit(data_unit);
    //             /* Mise à zéro de data_unit */
    //             for (size_t i = 0; i < 8; i++) {
    //                 for (size_t j = 0; j < 8; j++) {
    //                     data_unit[i][j] = 0;
    //                 }
    //             }
    //             count_horizontal = 0;
    //         }
    //         count_horizontal++;
    //     }
    // }

    /* Traitement des blocs Cb avec échantillonnage vertical */
    printf("%d %d\n", facteurs[H2], facteurs[V2]);
    for (size_t h = 0; h < facteurs[H1]; h++) {
        uint8_t count_vertical = 1;
        for (size_t v = 0; v < facteurs[V1]; v++) {
            for (size_t i = 0; i < 8; i++) {
                for (size_t j = 0; j < 8; j++) {
                    data_unit[i][j] = (uint8_t)(data_unit[i][j] + mcu->Cb[i + 8 * v][j + 8 * h] * facteurs[V2] / facteurs[V1]);
                }
            }
            printf("vertical %d\n", count_vertical);
            if (count_vertical == facteurs[V1] / facteurs[V2]) {
                /* Encodage de data_unit */
                afficher_data_unit(data_unit);
                /* Mise à zéro de data_unit */
                for (size_t i = 0; i < 8; i++) {
                    for (size_t j = 0; j < 8; j++) {
                        data_unit[i][j] = 0;
                    }
                }
                count_vertical = 0;
            }
            count_vertical++;
        }
    }

    
}

int main(int argc, char** argv) {
    if (argc != 8) {
        fprintf(stderr, "ERREUR: /.%s <Nom_Fichier> <h1> <h2> <h3> <v1> <v2> <v3>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    uint8_t facteurs[NOMBRE_FACTEURS];
    facteurs[H1] = (uint8_t)atoi(argv[2]);
    facteurs[H2] = (uint8_t)atoi(argv[3]);
    facteurs[H3] = (uint8_t)atoi(argv[4]);
    facteurs[V1] = (uint8_t)atoi(argv[5]);
    facteurs[V2] = (uint8_t)atoi(argv[6]);
    facteurs[V3] = (uint8_t)atoi(argv[7]);

    /*Ouverture du fichier*/
    FILE* fichier = ouvrir_fichier(argv[1]);

    /*Parsing en-tête*/
    image_ppm* image = parse_entete(fichier);

    MCUs* mcu = initialiser_MCUs(image, facteurs);

    printf("Largeur image : %d\n", image->largeur);
    printf("Hauteur image : %d\n", image->hauteur);
    printf("Largeur totale : %d\n", image->largeur_totale);
    printf("Hauteur totale : %d\n", image->hauteur_totale);
    printf("Plage de couleurs : %d\n", image->nb_couleurs);
    printf("Nombre de MCUs : %d\n", image->nb_MCUs);
    printf("Format : %2s\n", image->format);

    printf("Largeur_mcu = %d\n", mcu->largeur);
    printf("Hauteur_mcu = %d\n", mcu->hauteur);

    /* Allocation d'un bloc 8x8 */
    uint8_t** data_unit = calloc(8, sizeof *data_unit);
    for (size_t i = 0; i < 8; i++) {
        data_unit[i] = calloc(8, sizeof *data_unit[i]);
    }

    /*Traitement des MCUs*/
    for (size_t i = 0; i < image->nb_MCUs; i++) {
        printf("Traitement du mcu %ld\n", i + 1);
        recuperer_MCUs(fichier, image, mcu);
        afficher_MCUs(image->format, mcu);
        process_data_units(mcu, facteurs, data_unit);
    }

    /* Libération d'un bloc 8x8 */
    for (size_t i = 0; i < 8; i++) {
        free(data_unit[i]);
    }
    free(data_unit);

    /*Fermeture du fichier*/
    fermer_fichier(fichier);

    return EXIT_SUCCESS;
}
