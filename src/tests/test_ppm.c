#include <string.h>

#include "ppm.h"

/* Lecture d'un fichier ppm donné en argument*/
int main(int argc, char** argv) {
    /* Vérification d'un fichier en entrée */
    if (argc != 2) {
        printf("Entrez UN fichier ppm/pgm.\n");
        exit(EXIT_FAILURE);
    }

    printf("Nom du fichier : %s\n", argv[1]);

    image_ppm* image = lire_ppm(argv[1], 2, 2);
    printf("IMAGE : %p\n", image);
	printf("SUITE : (%d, %d)\n", image->hauteur, image->largeur);
	//afficher_image(image);
    //liberer_image(&image);

    return EXIT_SUCCESS;
}
