#include "../include/conversion.h"
#include "../include/ppm.h"

triplet_ycbcr* conversion(triplet_rgb* rgb) {
	triplet_ycbcr* ycbcr = (triplet_ycbcr*) malloc(sizeof(triplet_ycbcr));

	ycbcr->triplet[0] = 0.299 * (float) rgb->triplet[R] + 0.587 * (float) rgb->triplet[G] + 0.114 * (float) rgb->triplet[B];
	ycbcr->triplet[1] = -0.1687 * (float) rgb->triplet[R] - 0.3313 * (float) rgb->triplet[G] + 0.5000 * (float) rgb->triplet[B] + 128.0;
	ycbcr->triplet[2] = 0.5000 * (float) rgb->triplet[R] - 0.4187 * (float) rgb->triplet[G] - 0.0813 * (float) rgb->triplet[B] + 128.0;

	return ycbcr;
}

int main(void){
  triplet_rgb* rgb = (triplet_rgb*) malloc(sizeof(triplet_rgb));
  rgb->triplet[0] = 255;
	rgb->triplet[1] = 255;
	rgb->triplet[2] = 255;
  triplet_ycbcr* ycbcr = conversion(rgb);
	printf("Affichage des pixels en RGB :\n");
	printf("%d %d %d\n", rgb->triplet[R], rgb->triplet[G], rgb->triplet[B]);
	printf("Affichage des pixels en YCbCr :\n");
	printf("%f %f %f\n", ycbcr->triplet[0], ycbcr->triplet[1], ycbcr->triplet[2]);
}
