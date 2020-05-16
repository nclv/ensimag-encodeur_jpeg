#include "conversion.h"

int main(void) {
    pixel* pixel_test = (pixel*)malloc(sizeof(pixel_test));

    pixel_test->triplet_rgb[0] = 255;
    pixel_test->triplet_rgb[1] = 255;
    pixel_test->triplet_rgb[2] = 255;

    printf("Affichage des pixels en RGB :\n");
    printf("%i %i %i\n", pixel_test->triplet_rgb[0], pixel_test->triplet_rgb[1], pixel_test->triplet_rgb[2]);

    conversion(pixel_test);

    printf("Affichage des pixels en YCbCr :\n");
    printf("%i %i %i\n", pixel_test->triplet_ycbcr[0], pixel_test->triplet_ycbcr[1], pixel_test->triplet_ycbcr[2]);
}
