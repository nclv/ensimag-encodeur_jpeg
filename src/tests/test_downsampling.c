#include <stdio.h>
#include <stdlib.h>

#include "downsampling.h"
#include "ppm.h"

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
    sampling_factors[Cr][V] = (uint8_t)atoi(argv[7]);

    /* Ouverture du fichier */
    FILE* fichier = fopen(argv[1], "r");
    if (fichier == NULL) exit(EXIT_FAILURE);

    /*Parsing en-tête*/
    image_ppm* image = parse_entete(fichier);

    MCUs* mcu = initialiser_MCUs(image, sampling_factors);
    afficher_image(image);

    /* Allocation d'un bloc 8x8 */
    int16_t** data_unit = calloc(8, sizeof *data_unit);
    if (data_unit == NULL) exit(EXIT_FAILURE);
    for (size_t i = 0; i < 8; i++) {
        data_unit[i] = calloc(8, sizeof *data_unit[i]);
        if (data_unit[i] == NULL) exit(EXIT_FAILURE);
    }

    /* Traitement des MCUs */
    for (size_t i = 0; i < image->nb_MCUs; i++) {
        recuperer_MCUs(fichier, image, mcu);
        afficher_MCUs(image->nb_components, mcu);
        process_Y(mcu->Y, sampling_factors[Y][H], sampling_factors[Y][V], data_unit);
        if (image->nb_components == 3) {
            /* Mise à zéro de data_unit */
            for (size_t k = 0; k < 8; k++) {
                for (size_t j = 0; j < 8; j++) {
                    data_unit[k][j] = 0;
                }
            }
            printf("Passage à Cb\n");
            process_chroma(mcu->Cb, sampling_factors[Y][H], sampling_factors[Y][V], sampling_factors[Cb][H], sampling_factors[Cb][V], data_unit);
            printf("Passage à Cr\n");
            process_chroma(mcu->Cr, sampling_factors[Y][H], sampling_factors[Y][V], sampling_factors[Cr][H], sampling_factors[Cr][V], data_unit);
        }
    }

    /*Fermeture du fichier*/
    fclose(fichier);

    free_mcu(&mcu);
    free(image);

    /* Libération d'un bloc 8x8 */
    for (size_t i = 0; i < 8; i++) {
        free(data_unit[i]);
    }
    free(data_unit);

    return EXIT_SUCCESS;
}