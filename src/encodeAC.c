#include "encodeAC.h"
#include <math.h>
#include <stdint.h>

uint32_t encoderAC(uint8_t MCU_courante[8][8]) {
    int8_t valeur_a_encoder = (int8_t) MCU_courante_DC - MCU_ancienne_DC;
    uint8_t est_negatif = 0;
    /* on rend valeur_a_encoder positif et on met à jour le flag negatif */
    if (valeur_a_encoder < 0) {
        valeur_a_encoder = -valeur_a_encoder;
        est_negatif = 1;
    }
    /* la classe est obtenue avec le log2 */
    uint32_t classe_magnitude = ((uint32_t)log2(valeur_a_encoder))+1;
    /* l'indice se calcule facilement en faisant bien attention au flag negatif
       et en utilisant la classe */
    uint32_t valeur_sup = (uint32_t) pow(2, classe_magnitude) - 1;
    uint32_t indice = valeur_sup - valeur_a_encoder;
    if (est_negatif == 0) {
        uint32_t valeur_inf = (uint32_t) pow(2, classe_magnitude-1);
        indice = valeur_a_encoder - valeur_inf + valeur_sup - valeur_inf + 1;
    }
    printf("classe : %i\nindice : %i\n", classe_magnitude, indice);
}
