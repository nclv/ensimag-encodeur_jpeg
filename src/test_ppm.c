#include <string.h>

#include "ppm.h"

/* Lecture d'un fichier ppm donné en argument*/
int main(int argc, char** argv) {
    /* Vérification d'un fichier en entrée */
    if (argc != 2) {
        printf("Entrez UN fichier ppm/pgm.\n");
        return EXIT_FAILURE;
    }

    char* nom_fichier = (char*)malloc(20 * sizeof(char));
    if (nom_fichier == NULL) {
        printf("Erreur d'allocation\n");
        return EXIT_FAILURE;
    }
    printf("Nom du fichier : %s\n", argv[1]);

    image_ppm* image = lire_ppm(argv[1]);
    afficher_image(image);
    liberer_image(&image);

    return EXIT_SUCCESS;
}
