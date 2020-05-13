#include "conversion.h"

int main(void) {
	pixel* rgb = (pixel*)malloc(sizeof(pixel));

    rgb->triplet[0] = 255;
    rgb->triplet[1] = 255;
    rgb->triplet[2] = 255;

    pixel* ycbcr = conversion(rgb);

    printf("Affichage des pixels en RGB :\n");
    printf("%i %i %i\n", rgb->triplet[0], rgb->triplet[1], rgb->triplet[2]);

    printf("Affichage des pixels en YCbCr :\n");
    printf("%i %i %i\n", ycbcr->triplet[0], ycbcr->triplet[1], ycbcr->triplet[2]);
}
