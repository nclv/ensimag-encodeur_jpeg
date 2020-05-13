#include "../include/conversion.h"

int main(void) {
	triplet_rgb* rgb = (triplet_rgb*)malloc(sizeof(triplet_rgb));

    rgb->triplet[0] = 255;
    rgb->triplet[1] = 255;
    rgb->triplet[2] = 255;

    triplet_ycbcr* ycbcr = conversion(rgb);

    printf("Affichage des pixels en RGB :\n");
    printf("%d %d %d\n", rgb->triplet[R], rgb->triplet[G], rgb->triplet[B]);

    printf("Affichage des pixels en YCbCr :\n");
    printf("%f %f %f\n", ycbcr->triplet[0], ycbcr->triplet[1], ycbcr->triplet[2]);
}
