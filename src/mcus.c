#include "../include/mcus.h"

MCUs* recuperer_blocs(image_ppm* image) {
    MCUs* matrice = (MCUs*)malloc(sizeof(MCUs));
    uint32_t verticale = image->hauteur / TAILLE_BLOC;
    uint32_t horizontale = image->largeur / TAILLE_BLOC;

    if (image->hauteur % TAILLE_BLOC != 0) {
        verticale = (uint32_t)verticale + 1;
    }
    if (image->largeur % TAILLE_BLOC != 0) {
        horizontale = (uint32_t)horizontale + 1;
    }

    matrice->nombre_blocs = (uint32_t)verticale * horizontale;
    matrice->blocs = (uint32_t**)malloc(matrice->nombre_blocs * sizeof(uint32_t*));
    for (uint32_t i = 0; i < matrice->nombre_blocs; i++) {
        matrice->blocs[i] = (uint32_t*)malloc(2 * sizeof(uint32_t));
    }

    uint32_t courant = 0;
    for (uint32_t i = 0; i < verticale; i++) {
        for (uint32_t j = 0; j < horizontale; j++) {
            matrice->blocs[courant][0] = i * TAILLE_BLOC;
            matrice->blocs[courant][1] = j * TAILLE_BLOC;
            courant++;
        }
    }

    return matrice;
}

void afficher_MCUs(MCUs* matrice) {
    printf("Affichage des coordonnées des premiers éléments des blocs\n");
    printf("Nombre de blocs : %d\n", matrice->nombre_blocs);
    for (uint32_t i = 0; i < matrice->nombre_blocs; i++) {
        printf("(%d, %d)\n", matrice->blocs[i][0], matrice->blocs[i][1]);
    }
}
