#include <stdio.h>
#include <stdlib.h>

#include "ppm.h"
#include "downsampling.h"

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

    /* Ouverture du fichier */
    FILE* fichier = fopen(argv[1], "r");
    if (fichier == NULL) exit(EXIT_FAILURE);

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
        // process_Y(mcu->Y, sampling_factors[Y][H], sampling_factors[Y][V], data_unit);
        process_chroma(mcu->Cb, sampling_factors[Y][H], sampling_factors[Y][V], sampling_factors[Cb][H], sampling_factors[Cb][V], data_unit);
        // process_chroma(mcu->Cr, sampling_factors[Y][H], sampling_factors[Y][V], sampling_factors[Cr][H], sampling_factors[Cr][V], data_unit);
    }

    /* Libération d'un bloc 8x8 */
    for (size_t i = 0; i < 8; i++) {
        free(data_unit[i]);
    }
    free(data_unit);

    /*Fermeture du fichier*/
    fclose(fichier);

    return EXIT_SUCCESS;
}
