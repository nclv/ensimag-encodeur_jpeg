#include "conversion.h"

int main(void) {

		pixel* pixel = (pixel*)malloc(sizeof(pixel));

    pixel->triplet_rgb[0] = 255;
    pixel->triplet_rgb[1] = 255;
		pixel->triplet_rgb[2] = 255;

		printf("Affichage des pixels en RGB :\n");
    printf("%i %i %i\n", pixel->triplet_rgb[0], pixel->triplet_rgb[1], pixel->triplet_rgb[2]);


		conversion(pixel);


    printf("Affichage des pixels en YCbCr :\n");
    printf("%i %i %i\n", pixel->triplet_ycbcr[0], pixel->triplet_ycbcr[1], pixel->triplet_ycbcr[2]);
}
