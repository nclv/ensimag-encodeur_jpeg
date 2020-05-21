#include "encodeDC.h"

#include <math.h>
#include <stdint.h>

static uint8_t get_magnitude(int16_t freq) {
    /* On passe en positif*/
    freq = freq > 0 ? freq: -freq;
    uint8_t classe = 0;
    while (freq > 0) {
        freq >>= 1;
        classe++;
    }
    return classe;
}

uint32_t encoderDC(int8_t MCU_courante_DC, int8_t MCU_ancienne_DC) {
    /* On code la différence entre valeurs DC de deux blocs consécutifs */
    int8_t valeur_a_encoder = (int8_t)MCU_courante_DC - MCU_ancienne_DC;
    uint8_t est_negatif = 0;
    /* on rend valeur_a_encoder positif et on met à jour le flag negatif */
    if (valeur_a_encoder < 0) {
        valeur_a_encoder = -valeur_a_encoder;
        est_negatif = 1;
    }
    /* la classe est obtenue avec le log2 */
    uint32_t classe_magnitude = ((uint32_t)log2(valeur_a_encoder)) + 1;
    /* l'indice se calcule facilement en faisant bien attention au flag negatif
       et en utilisant la classe */
    uint32_t valeur_sup = (uint32_t)pow(2, classe_magnitude) - 1;
    uint32_t indice = valeur_sup - valeur_a_encoder;
    if (est_negatif == 0) {
        uint32_t valeur_inf = (uint32_t)pow(2, classe_magnitude - 1);
        indice = valeur_a_encoder;
    }
    /* la classe devra être codée sur 4 bits et l'indice sur la valeur de la classe bits
       par exemple, 77 classe 7 indice 77 donc sur 4 + 7 = 11 bits */
    printf("classe : %i\nindice : %i\n", classe_magnitude, indice);
}
