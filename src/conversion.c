#include "../include/conversion.h"
#include "../include/ppm.h"

triplet_ycbcr* conversion(triplet_rgb* rgb) {
	triplet_ycbcr* ycbcr;

	ycbcr->triplet[0] = (float)(0.299 * rgb->triplet[R] + 0.587 * rgb->triplet[G] + 0.114 * rgb->triplet[B]);
	ycbcr->triplet[1] = (float)(-0.1687 * rgb->triplet[R] - 0.3313 * rgb->triplet[G] + 0.5000 * rgb->triplet[B] + 128);
	ycbcr->triplet[2] = (float)(0.5000 * rgb->triplet[R] - 0.4187 * rgb->triplet[G] - 0.0813 * rgb->triplet[B] + 128);

	return ycbcr;
}

int main(void){
  triplet_rgb* rgb;
  rgb->triplet[0] = 255;
	rgb->triplet[1] = 255;
	rgb->triplet[2] = 255;
  triplet_ycbcr* ycbcr = conversion(rgb);
	printf("Affichage des pixels en RGB :\n");
	printf("%d %d %d\n", rgb->triplet[R], rgb->triplet[G], rgb->triplet[B]);
	printf("Affichage des pixels en YCbCr :\n");
	printf("%d %d %d\n", ycbcr->triplet[0], ycbcr->triplet[1], ycbcr->triplet[2]);
}
