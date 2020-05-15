#include "../include/zigzag.h"

/* utilise les diagonales de la matrice
 * si on est sur une diagonale paire, le sens de remplissage est vers le haut
 * si on est sur une diagonale impaire, le sens de remplissage est vers le bas
 */
tableau zigzag1(bloc matrice) {
  tableau tab;

  int i = 0;
  int j = 0;

  int diag = 0;

  int r = 0;

  while (diag < 8) {

    if (diag % 2 == 0){
      i = diag;
      while (i > 0) {
        j = diag - i;
        tab.tableau[r] = matrice.bloc[i][j];
        r++;
        i--;
      }

    } else {
      while (i <= diag) {
        j = diag - i;
        tab.tableau[r] = matrice.bloc[i][j];
        r++;
        i--;
      }
    }
    diag ++;
  }

  while (diag < 15){

    if (diag % 2 == 0){
      i = 7;
      while (i > 0) {
        j = diag - i;
        tab.tableau[r] = matrice.bloc[i][j];
        r++;
        i--;
      }

    } else {
      j = 7;
      while (j >= i) {
        i = diag - j;
        tab.tableau[r] = matrice.bloc[i][j];
        r++;
        j--;
      }
    }
    diag ++;
  }
  return tab;
}

tableau zigzag2(bloc matrice) {

  tableau tab;
  tab.tableau[0] = matrice.bloc[0][0];

  tab.tableau[1] = matrice.bloc[0][1];
  tab.tableau[2] = matrice.bloc[1][0];

  tab.tableau[3] = matrice.bloc[2][0];
  tab.tableau[4] = matrice.bloc[1][1];
  tab.tableau[5] = matrice.bloc[0][2];

  tab.tableau[6] = matrice.bloc[0][3];
  tab.tableau[7] = matrice.bloc[1][2];
  tab.tableau[8] = matrice.bloc[2][1];
  tab.tableau[9] = matrice.bloc[3][0];

  tab.tableau[10] = matrice.bloc[4][0];
  tab.tableau[11] = matrice.bloc[3][1];
  tab.tableau[12] = matrice.bloc[2][2];
  tab.tableau[13] = matrice.bloc[1][3];
  tab.tableau[14] = matrice.bloc[0][4];

  tab.tableau[15] = matrice.bloc[0][5];
  tab.tableau[16] = matrice.bloc[1][4];
  tab.tableau[17] = matrice.bloc[2][3];
  tab.tableau[18] = matrice.bloc[3][2];
  tab.tableau[19] = matrice.bloc[4][1];
  tab.tableau[20] = matrice.bloc[5][0];

  tab.tableau[21] = matrice.bloc[6][0];
  tab.tableau[22] = matrice.bloc[5][1];
  tab.tableau[23] = matrice.bloc[4][2];
  tab.tableau[24] = matrice.bloc[3][3];
  tab.tableau[25] = matrice.bloc[2][4];
  tab.tableau[26] = matrice.bloc[1][5];
  tab.tableau[27] = matrice.bloc[0][6];

  tab.tableau[28] = matrice.bloc[0][7];
  tab.tableau[29] = matrice.bloc[1][6];
  tab.tableau[30] = matrice.bloc[2][5];
  tab.tableau[32] = matrice.bloc[3][4];
  tab.tableau[32] = matrice.bloc[4][3];
  tab.tableau[33] = matrice.bloc[5][2];
  tab.tableau[34] = matrice.bloc[6][1];
  tab.tableau[35] = matrice.bloc[7][0];

  tab.tableau[36] = matrice.bloc[7][1];
  tab.tableau[37] = matrice.bloc[6][2];
  tab.tableau[38] = matrice.bloc[5][3];
  tab.tableau[39] = matrice.bloc[4][4];
  tab.tableau[40] = matrice.bloc[3][5];
  tab.tableau[41] = matrice.bloc[2][6];
  tab.tableau[42] = matrice.bloc[1][7];

  tab.tableau[43] = matrice.bloc[2][7];
  tab.tableau[44] = matrice.bloc[3][6];
  tab.tableau[45] = matrice.bloc[4][5];
  tab.tableau[46] = matrice.bloc[5][4];
  tab.tableau[47] = matrice.bloc[6][3];
  tab.tableau[48] = matrice.bloc[7][2];

  tab.tableau[49] = matrice.bloc[7][3];
  tab.tableau[50] = matrice.bloc[6][4];
  tab.tableau[51] = matrice.bloc[5][5];
  tab.tableau[52] = matrice.bloc[4][6];
  tab.tableau[53] = matrice.bloc[3][7];

  tab.tableau[54] = matrice.bloc[4][7];
  tab.tableau[55] = matrice.bloc[5][6];
  tab.tableau[56] = matrice.bloc[6][5];
  tab.tableau[57] = matrice.bloc[7][4];

  tab.tableau[58] = matrice.bloc[7][5];
  tab.tableau[59] = matrice.bloc[6][6];
  tab.tableau[60] = matrice.bloc[5][7];

  tab.tableau[61] = matrice.bloc[6][7];
  tab.tableau[62] = matrice.bloc[7][6];

  tab.tableau[63] = matrice.bloc[7][7];

  return tab;
}
