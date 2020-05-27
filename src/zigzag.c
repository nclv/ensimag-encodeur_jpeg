#include "zigzag.h"

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void zigzag_inplace(int16_t matrice[8][8]) {
    /*Table des indices zig-zag*/
    static uint8_t indices_zigzag[64] = {
      0, 1, 8, 16, 9, 2, 3, 10,
      17, 24, 32, 25, 18, 11, 4, 5,
      12, 19, 26, 33, 40, 48, 41, 34,
      27, 20, 13, 6, 7, 14, 21, 28,
      35, 42, 49, 56, 57, 50, 43, 36,
      29, 22, 15, 23, 30, 37, 44, 51,
      58, 59, 52, 45, 38, 31, 39, 46,
      53, 60, 61, 54, 47, 55, 62, 63};

    /* Allocation plus complexe avec une matrice */
    int16_t tmp[64];

    /* Copie de la matrice */
    size_t inc1 = 0;
    for (size_t i = 0; i < 8; i++) {
        for (size_t j = 0; j < 8; j++) {
            tmp[i + j + inc1] = matrice[i][j];
        }
        inc1 += 7;
    }

    size_t inc = 0;
    for (size_t i = 0; i < 8; i++) {
        for (size_t j = 0; j < 8; j++) {
            matrice[i][j] = tmp[indices_zigzag[i + j + inc]];
        }
        inc += 7;
    }
}

void zigzag(int16_t matrice[8][8], int16_t tableau[64]) {
    tableau[0] = matrice[0][0];

    tableau[1] = matrice[0][1];
    tableau[2] = matrice[1][0];

    tableau[3] = matrice[2][0];
    tableau[4] = matrice[1][1];
    tableau[5] = matrice[0][2];

    tableau[6] = matrice[0][3];
    tableau[7] = matrice[1][2];
    tableau[8] = matrice[2][1];
    tableau[9] = matrice[3][0];

    tableau[10] = matrice[4][0];
    tableau[11] = matrice[3][1];
    tableau[12] = matrice[2][2];
    tableau[13] = matrice[1][3];
    tableau[14] = matrice[0][4];

    tableau[15] = matrice[0][5];
    tableau[16] = matrice[1][4];
    tableau[17] = matrice[2][3];
    tableau[18] = matrice[3][2];
    tableau[19] = matrice[4][1];
    tableau[20] = matrice[5][0];

    tableau[21] = matrice[6][0];
    tableau[22] = matrice[5][1];
    tableau[23] = matrice[4][2];
    tableau[24] = matrice[3][3];
    tableau[25] = matrice[2][4];
    tableau[26] = matrice[1][5];
    tableau[27] = matrice[0][6];

    tableau[28] = matrice[0][7];
    tableau[29] = matrice[1][6];
    tableau[30] = matrice[2][5];
    tableau[32] = matrice[3][4];
    tableau[32] = matrice[4][3];
    tableau[33] = matrice[5][2];
    tableau[34] = matrice[6][1];
    tableau[35] = matrice[7][0];

    tableau[36] = matrice[7][1];
    tableau[37] = matrice[6][2];
    tableau[38] = matrice[5][3];
    tableau[39] = matrice[4][4];
    tableau[40] = matrice[3][5];
    tableau[41] = matrice[2][6];
    tableau[42] = matrice[1][7];

    tableau[43] = matrice[2][7];
    tableau[44] = matrice[3][6];
    tableau[45] = matrice[4][5];
    tableau[46] = matrice[5][4];
    tableau[47] = matrice[6][3];
    tableau[48] = matrice[7][2];

    tableau[49] = matrice[7][3];
    tableau[50] = matrice[6][4];
    tableau[51] = matrice[5][5];
    tableau[52] = matrice[4][6];
    tableau[53] = matrice[3][7];

    tableau[54] = matrice[4][7];
    tableau[55] = matrice[5][6];
    tableau[56] = matrice[6][5];
    tableau[57] = matrice[7][4];

    tableau[58] = matrice[7][5];
    tableau[59] = matrice[6][6];
    tableau[60] = matrice[5][7];

    tableau[61] = matrice[6][7];
    tableau[62] = matrice[7][6];

    tableau[63] = matrice[7][7];
}

void afficher_zigzag(int16_t zigzag[8][8]) {
  for (uint8_t k = 0; k < 8; k++) {
      for (uint8_t l = 0; l < 8; l++) {
          printf("%04X", zigzag[k][l]);
      }
      printf("\n");
  }
}
