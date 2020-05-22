#include "encode_ACDC.h"

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

uint32_t encode_DC_freq(int8_t difference_DC) {
    /* On code la différence entre valeurs DC de deux blocs consécutifs */
    uint32_t classe_magnitude = get_magnitude(difference_DC);
    unsigned int indice = abs(difference_DC);
    if (difference_DC < 0) {
        indice = (1 << classe_magnitude) - indice;
    }
    /* la classe devra être codée sur 4 bits et l'indice sur la valeur de la classe bits
       par exemple, 77 classe 7 indice 77 donc sur 4 + 7 = 11 bits */
    printf("classe : %i\nindice : %i\n", classe_magnitude, indice);
}

uint32_t encoderAC(int8_t MCU_coefficient, uint8_t nb_zeros_enchaines) {
    /* déclaration des variables comptant les zéros et déterminant les
       symboles à coder pour les coefficients non nuls */
    uint8_t zeros_count = nb_zeros_enchaines;
    uint8_t nombre_16_zeros = 0;
    uint8_t nombre_zero_restants = 0;
    if (MCU_coefficient == 0) {
        zeros_count++;
    } else {
        nombre_16_zeros = zeros_count / 16;
        nombre_zero_restants = zeros_count % 16;
        zeros_count = 0;
        printf("coef : %i\n", MCU_coefficient);
        /* même algo de classe | indice que pour l'encodage DC */
        uint32_t classe_magnitude = get_magnitude(MCU_coefficient);
        unsigned int indice = abs(MCU_coefficient);
        uint8_t valeur_sup = 1 << classe_magnitude;
        if (MCU_coefficient < 0) {
            indice = valeur_sup - indice;  // complémentaire
        }
        printf("classe : %i\nindice : %i\n", classe_magnitude, indice);
        printf("16 zeros : %i\nzeros restants : %i\n", nombre_16_zeros, nombre_zero_restants);
        printf("-----------------------\n");
        /* on doit coder dansle bitstream nombre_16_zeros * 0xF0
           suivi de 0xnombre_zero_restantsClasseMagnitude suivi
           de l'indice codé sur ClasseMagnitude bits comme dans
           l'encodage DC */
    }
}

/*
int main()
{
    int8_t matrice[8][8]  = {{77, 45, 120, 16, -44, -16, 0, 4},
                             {-64, 4, 112, 10, -9, 0, 0, 0},
                             {1, 7, 3, -11, -16, 2, 0, 1},
                             {7, 0, 0, 0, 0, 0, 0, 0},
                             {6, -11, 0, 0, 0, 0, 0, 28},
                             {0, 0, 0, 0, 0, 0, 1, 0},
                             {0, 0, 0, 0, 0, 0, 0, 0},
                             {0, 0, 0, 0, 0, 0, 0, 0}};
    for (uint8_t k=0;k<8;k++) {
      for (uint8_t l=0;l<8;l++) {
        encoderAC(matrice[k][l]);
      }
    }
    return 0;
}
*/
