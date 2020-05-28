#include <stdio.h>
#include <stdlib.h>

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
    sampling_factors[Cb][V] = (uint8_t)atoi(argv[7]);

    /* Ouverture du fichier */
    FILE* fichier = fopen(argv[1], "r");
    if (fichier == NULL) exit(EXIT_FAILURE);

    /*Parsing en-tête*/
    image_ppm* image = parse_entete(fichier);
    afficher_image(image);

    MCUs* mcu = initialiser_MCUs(image, sampling_factors);

    printf("Largeur_mcu = %d\n", mcu->largeur);
    printf("Hauteur_mcu = %d\n", mcu->hauteur);

    /* Traitement des MCUs */
    for (size_t i = 0; i < image->nb_MCUs; i++) {
        printf("Traitement du mcu %ld\n", i + 1);
        recuperer_MCUs(fichier, image, mcu);
        afficher_MCUs(image->nb_components, mcu);
    }

    /*Fermeture du fichier*/
    fclose(fichier);

    return EXIT_SUCCESS;
}
