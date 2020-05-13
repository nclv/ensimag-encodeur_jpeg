#include <string.h>

#include "ppm.h"

int main(int argc, char** argv) {
    char* nom_fichier = (char*)malloc(20 * sizeof(char));
    (void) argc;
    strcpy(nom_fichier, argv[1]);
    printf("Nom du fichier : %s\n", nom_fichier);

    image_ppm* image = lire_ppm(nom_fichier);
    afficher_image(image);

    return EXIT_SUCCESS;
}
