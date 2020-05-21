#include "encodeDC.h"

#include <stdint.h>
#include <stdlib.h>  // abs()

static uint8_t get_magnitude(int16_t freq) {
    /* On passe en positif*/
    freq = freq > 0 ? freq : -freq;
    uint8_t classe = 0;
    while (freq > 0) {
        freq >>= 1;
        classe++;
    }
    return classe;
}

uint32_t encoderDC(int8_t MCU_courante_DC, int8_t MCU_ancienne_DC) {
    /* On code la différence entre valeurs DC de deux blocs consécutifs */
    int8_t difference_DC = (int8_t)(MCU_courante_DC - MCU_ancienne_DC);
    uint32_t classe_magnitude = get_magnitude(difference_DC);
    unsigned int indice = abs(difference_DC);
    if (difference_DC < 0) {
        indice = ~indice;  // complémentaire
    }
    /* la classe devra être codée sur 4 bits et l'indice sur la valeur de la classe bits
       par exemple, 77 classe 7 indice 77 donc sur 4 + 7 = 11 bits */
    printf("classe : %i\nindice : %i\n", classe_magnitude, indice);
}
