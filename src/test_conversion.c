#include "../include/conversion.h"

int main(void) {
	pixel_rgb* rgb = (pixel_rgb*)malloc(sizeof(pixel_rgb));

    rgb->r = 255;
    rgb->g = 255;
    rgb->b = 255;

    pixel_ycbcr* ycbcr = conversion(rgb);

    printf("Affichage des pixels en RGB :\n");
    printf("%d %d %d\n", rgb->r, rgb->g, rgb->b);

    printf("Affichage des pixels en YCbCr :\n");
    printf("%f %f %f\n", ycbcr->y, ycbcr->cb, ycbcr->cr);
}
