#include "encodeAC.h"
#include <math.h>
#include <stdint.h>
#include <stdio.h>



uint32_t encoderAC(int8_t MCU_courante[8][8]) {
    /* déclaration des variables comptant les zéros et déterminant les
       symboles à coder pour les coefficients non nuls */
    uint8_t zeros_count = 0;
    uint8_t nombre_16_zeros = 0;
    uint8_t nombre_zero_restants = 0;
    for (uint8_t k = 0; k<8; k++) {
        for (uint8_t l = 0; l<8; l++) {
            /* on passe la valeur moyenne [0][0] qui à déjà été encodée et ne doit
               pas être prise en compte ici */
            if (k  != 0 || l != 0) {
                int8_t MCU_coefficient = MCU_courante[k][l];
                if (MCU_coefficient == 0) {
                    zeros_count++;
                }
                /* on a trouvé un coefficient non nul */
                else {
                    nombre_16_zeros = zeros_count/16;
                    nombre_zero_restants = zeros_count%16;
                    zeros_count = 0;
                    printf("coef : %i\n", MCU_coefficient);
                    /* même algo de classe | indice que pour l'encodage DC */
                    uint8_t est_negatif = 0;
                    if (MCU_coefficient < 0) {
                        MCU_coefficient = -MCU_coefficient;
                        est_negatif = 1;
                    }
                    uint32_t classe_magnitude = ((uint32_t)log2(MCU_coefficient))+1;
                    uint32_t valeur_sup = (uint32_t) pow(2, classe_magnitude) - 1;
                    uint32_t indice = valeur_sup - MCU_coefficient;
                    if (est_negatif == 0) {
                        uint32_t valeur_inf = (uint32_t) pow(2, classe_magnitude-1);
                        indice = MCU_coefficient;
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
        }
    }
    if (zeros_count != 0) {
        printf("end of block reached\n");
        /* coder dans le bitstream ENDOFBLOCK */
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
    encoderAC(matrice);
    return 0;
}
*/
