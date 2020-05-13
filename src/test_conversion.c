#include "../include/conversion.h"

int main(void) {
	pixel* pixel = (pixel*)malloc(sizeof(pixel));

    pixel->triplet[0] = 255;
    pixel->triplet[1] = 255;
		pixel->triplet[2] = 255;


		printf("Affichage des pixels en RGB :\n");
		printf("%f %f %f\n", ycbcr->y, ycbcr->cb, ycbcr->cr);

		conversion(pixel);


    printf("Affichage des pixels en YCbCr :\n");
    printf("%f %f %f\n", ycbcr->y, ycbcr->cb, ycbcr->cr);
}
