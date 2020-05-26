#include "encode_ACDC.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>  // abs()

#include "bitstream.h"
#include "huffman.h"

static uint8_t get_magnitude(int16_t freq) {
    /* On passe en positif*/
    freq = (int16_t)(freq > 0 ? freq : -freq);
    uint8_t classe = 0;
    while (freq > 0) {
        freq >>= 1;
        classe++;
    }
    return classe;
}

static void encode_DC_freq(bitstream *stream, huff_table *dc_table, int16_t difference_DC) {
    /* On code la différence entre valeurs DC de deux blocs consécutifs */
    uint8_t classe_magnitude = get_magnitude(difference_DC);
    uint16_t indice = (uint16_t)abs(difference_DC);
    if (difference_DC < 0) {
        indice = (uint16_t)((1 << classe_magnitude) - indice);
    }

    uint8_t nb_bits_magnitude = 0;
    uint32_t code_magnitude = huffman_table_get_path(dc_table, classe_magnitude, &nb_bits_magnitude);
    bitstream_write_bits(stream, code_magnitude, nb_bits_magnitude, false);
    bitstream_write_bits(stream, indice, classe_magnitude, false);
    /* la classe devra être codée sur 4 bits et l'indice sur la valeur de la classe bits
       par exemple, 77 classe 7 indice 77 donc sur 4 + 7 = 11 bits */
    printf("classe : %i\nindice : %i\n", classe_magnitude, indice);
}

static void encoder_AC_freq(bitstream *stream, huff_table *ac_table, int16_t freq_AC, uint8_t zeros_count) {
    uint8_t classe_magnitude = get_magnitude(freq_AC);
    uint16_t indice = (uint16_t)abs(freq_AC);
    if (freq_AC < 0) {
        indice = (uint16_t)((1 << classe_magnitude) - indice);
    }
    /* 
        On doit coder le nombre de coefficients nuls puis la classe de magnitude 
        On écrit un octet.
        Bitshifts the input 4 bits to the left, then masks by the lower 4 bits.
        Le deuxième masquage est sécuritaire.
    */
    uint8_t value = ((zeros_count << 4) & 0xf0) | (classe_magnitude & 0x0f);
    uint8_t nb_bits_zeros_magnitude = 0;
    uint32_t code_magnitude = huffman_table_get_path(ac_table, value, &nb_bits_zeros_magnitude);
    bitstream_write_bits(stream, code_magnitude, nb_bits_zeros_magnitude, false);
    bitstream_write_bits(stream, indice, classe_magnitude, false);

    printf("coef : %i\n", freq_AC);
    printf("classe : %i\nindice : %i\n", classe_magnitude, indice);
}

void ecrire_coeffs(bitstream *stream, int16_t data_unit[8][8], huff_table *dc_table, huff_table *ac_table, int16_t difference_DC) {
    uint8_t zeros_count = 0;
    int16_t difference_DC = data_unit[0][0] - difference_DC;

    uint8_t last_non_zero_line = 0;
    uint8_t last_non_zero_col = 0;
    for (size_t i = 7; i >= 0; i--) {
        for (size_t j = 7; j >= 0; j--) {
            if (data_unit[i][j] != 0) {
                last_non_zero_line = i;
                last_non_zero_col = j;
                goto got_last_non_zero;
            }
        }
    }
    // We use goto to break of 2 loops
    got_last_non_zero:

    for (size_t i = 0; i < 8; i++) {
        for (size_t j = 0; j < 8; j++) {
            if (i == last_non_zero_line && j == last_non_zero_col) {
                goto writeEOB;
            }
            if (i == 0 && j == 0) {
                encode_DC_freq(stream, dc_table, difference_DC);
                continue;
            }
            if (data_unit[i][j] == 0) {
                zeros_count++;
                if (zeros_count == 16) {
                    // Write ZRL
                    uint8_t nb_bits_ZRL = 0;
                    uint32_t code_ZRL = huffman_table_get_path(ac_table, 0xf0, &nb_bits_ZRL);
                    bitstream_write_bits(stream, code_ZRL, nb_bits_ZRL, true);
                    zeros_count = 0;
                }
            } else {
                encoder_AC_freq(stream, ac_table, data_unit[i][j], zeros_count);
                zeros_count = 0;
            }
        }
    }
    // Write EOB
    writeEOB:
    uint8_t nb_bits_EOB = 0;
    uint32_t code_EOB = huffman_table_get_path(ac_table, 0x00, &nb_bits_EOB);
    bitstream_write_bits(stream, code_EOB, nb_bits_EOB, true);
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
