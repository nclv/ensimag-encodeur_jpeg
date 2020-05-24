#include <stdio.h>
#include <stdlib.h>

#include "ppm.h"

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

    MCUs* bloc = initialiser_MCUs(image, facteurs);

    printf("Largeur image : %d\n", image->largeur);
    printf("Hauteur image : %d\n", image->hauteur);
    printf("Largeur totale : %d\n", image->largeur_totale);
    printf("Hauteur totale : %d\n", image->hauteur_totale);
    printf("Plage de couleurs : %d\n", image->nb_couleurs);
    printf("Nombre de MCUs : %d\n", image->nb_MCUs);
    printf("Format : %2s\n", image->format);

    printf("Largeur_bloc = %d\n", bloc->largeur);
    printf("Hauteur_bloc = %d\n", bloc->hauteur);

    /*Traitement des MCUs*/
    for (size_t i = 0; i < image->nb_MCUs; i++) {
        printf("Traitement du bloc %ld\n", i + 1);
        recuperer_MCUs(fichier, image, bloc);
        afficher_MCUs(image, bloc);
    }

    /*Fermeture du fichier*/
    fermer_fichier(fichier);

    return EXIT_SUCCESS;
}
