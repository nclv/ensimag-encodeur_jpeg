#include "conversion.h"

int main(void) {
<<<<<<< HEAD
	pixel* pixel = (pixel*)malloc(sizeof(pixel));

    pixel->triplet[0] = 255;
    pixel->triplet[1] = 255;
		pixel->triplet[2] = 255;


		printf("Affichage des pixels en RGB :\n");
		printf("%f %f %f\n", ycbcr->y, ycbcr->cb, ycbcr->cr);

		conversion(pixel);

=======
	pixel* rgb = (pixel*)malloc(sizeof(pixel));

    rgb->triplet[0] = 255;
    rgb->triplet[1] = 255;
    rgb->triplet[2] = 255;

    pixel* ycbcr = conversion(rgb);

    printf("Affichage des pixels en RGB :\n");
    printf("%i %i %i\n", rgb->triplet[0], rgb->triplet[1], rgb->triplet[2]);
>>>>>>> 8c7cce2d90d744a4cd05f11eb9e779cbc5e0daaf

    printf("Affichage des pixels en YCbCr :\n");
    printf("%i %i %i\n", ycbcr->triplet[0], ycbcr->triplet[1], ycbcr->triplet[2]);
}
