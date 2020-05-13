#include "../include/mcus.h"
#include "../include/ppm.h"

int main(int argc, char** argv) {
    char* nom_fichier = (char*)malloc(20 * sizeof(char));
    strcpy(nom_fichier, argv[1]);
    printf("Nom du fichier : %s\n", nom_fichier);

    image_ppm* image = lire_ppm(nom_fichier);

    MCUs* matrice = recuperer_blocs(image);
    afficher_MCUs(matrice);

    return EXIT_SUCCESS;
}
