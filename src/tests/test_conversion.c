#include "conversion.h"

int main(void) {
    pixel* pixel_test = (pixel*)malloc(sizeof(pixel));

    conversion(pixel_test, 255, 255, 255);

    printf("Affichage des pixels en YCbCr :\n");
    printf("%i %i %i\n", pixel_test->couleurs[Y], pixel_test->couleurs[CB], pixel_test->couleurs[CR]);
}
