#include <stdio.h>
#include <stdlib.h>

#include "ppm.h"

void afficher_data_unit(int16_t** data_unit) {
    printf("Data Unit\n");
    for (size_t i = 0; i < 8; i++) {
        for (size_t j = 0; j < 8; j++) {
            printf("%02hhX ", data_unit[i][j]);
        }
        printf("\n");
    }
}

void process_Y(int16_t** Y_mcu, uint8_t h1, uint8_t v1, int16_t** data_unit) {
    /* Traitement des blocs Y */
    printf("%d %d\n", v1, h1);
    for (size_t v = 0; v < v1; v++) {
        for (size_t h = 0; h < h1; h++) {
            for (size_t i = 0; i < 8; i++) {
                for (size_t j = 0; j < 8; j++) {
                    data_unit[i][j] = Y_mcu[i + 8 * v][j + 8 * h];
                }
            }
            /* Encodage de data_unit */
            afficher_data_unit(data_unit);
        }
    }
}

void process_chroma(int16_t** chroma_mcu, uint8_t h1, uint8_t v1, uint8_t h_chroma, uint8_t v_chroma, int16_t** data_unit) {
    /* Traitement des blocs Cb / Cr avec échantillonnage horizontal */
    // printf("%d \n", v1);
    // printf("%d %d\n", h_chroma, v_chroma);
    float remainder = 0;
    uint8_t chroma_value = 0;
    uint8_t h_div = (h1 / h_chroma);
    uint8_t v_div = (v1 / v_chroma);
    // printf("%d %d\n", h_div, v_div);
    /* On boucle dans l'ordre des DU à traiter */
    for (size_t v = 0; v < v_chroma; v++) {
        for (size_t h = 0; h < h_chroma; h++) {
            printf("%ld, %ld\n", v, h);
            /* Echantillonnage de la chrominance */
            for (size_t i = 0; i < 8; i++) {
                for (size_t j = 0; j < 8; j++) {
                    for (size_t k = 0; k < h_div; k++) {
                        for (size_t l = 0; l < v_div; l++) {
                            chroma_value = (uint8_t)chroma_mcu[v_div * i + l + 8 * v][h_div * j + k + 8 * h];
                            data_unit[i][j] = (uint8_t)(data_unit[i][j] + chroma_value / (h_div * v_div));
                            remainder += (float)(chroma_value % (h_div * v_div));
                        }
                    }
                    data_unit[i][j] = (int16_t)(data_unit[i][j] + remainder / (float)(h_div * v_div));
                    remainder = 0;
                }
            }
            /* Traitement des DUs */
            afficher_data_unit(data_unit);
            // à faire pdt l'écriture dans le bitstream.
            /* Mise à zéro de data_unit */
            for (size_t i = 0; i < 8; i++) {
                for (size_t j = 0; j < 8; j++) {
                    data_unit[i][j] = 0;
                }
            }
        }
    }
}

void process_data_units(MCUs* mcu, uint8_t sampling_factors[NB_COLOR_COMPONENTS][NB_DIRECTIONS], int16_t** data_unit) {
    // process_Y(mcu->Y, sampling_factors[Y][H], sampling_factors[Y][V], data_unit);
    process_chroma(mcu->Cb, sampling_factors[Y][H], sampling_factors[Y][V], sampling_factors[Cb][H], sampling_factors[Cb][V], data_unit);
    // process_chroma(mcu->Cr, sampling_factors[Y][H], sampling_factors[Y][V], sampling_factors[Cr][H], sampling_factors[Cr][V], data_unit);
}

int main(int argc, char** argv) {
    if (argc != 8) {
        fprintf(stderr, "ERREUR: /.%s <Nom_Fichier> <h1> <h2> <h3> <v1> <v2> <v3>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    uint8_t sampling_factors[NB_COLOR_COMPONENTS][NB_DIRECTIONS];
    sampling_factors[Y][H] = (uint8_t)atoi(argv[2]);
    sampling_factors[Cb][H] = (uint8_t)atoi(argv[3]);
    sampling_factors[Cr][H] = (uint8_t)atoi(argv[4]);
    sampling_factors[Y][V] = (uint8_t)atoi(argv[5]);
    sampling_factors[Cb][V] = (uint8_t)atoi(argv[6]);
    sampling_factors[Cb][V] = (uint8_t)atoi(argv[7]);

    /*Ouverture du fichier*/
    FILE* fichier = ouvrir_fichier(argv[1]);

    /*Parsing en-tête*/
    image_ppm* image = parse_entete(fichier);

    MCUs* mcu = initialiser_MCUs(image, sampling_factors);

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
    int16_t** data_unit = calloc(8, sizeof *data_unit);
    if (data_unit == NULL) exit(EXIT_FAILURE);
    for (size_t i = 0; i < 8; i++) {
        data_unit[i] = calloc(8, sizeof *data_unit[i]);
        if (data_unit[i] == NULL) exit(EXIT_FAILURE);
    }

    /* Traitement des MCUs */
    for (size_t i = 0; i < image->nb_MCUs; i++) {
        printf("Traitement du mcu %ld\n", i + 1);
        recuperer_MCUs(fichier, image, mcu);
        afficher_MCUs(image->format, mcu);
        process_data_units(mcu, sampling_factors, data_unit);
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
