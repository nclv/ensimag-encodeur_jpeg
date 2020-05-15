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
