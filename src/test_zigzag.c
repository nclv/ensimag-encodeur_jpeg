#include "../include/zigzag.h"

int main(void){
  // Matrice 8x8 de nombres aléatoirs entre 0 et 255
  bloc matrice;

  int nb = 0;

  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      matrice.bloc[i][j] = nb;
      nb ++;
      printf("%u \n", matrice.bloc[i][j]);
    }
  }

  // zigzag de la matrice
  tableau tab = zigzag2(matrice);
  tableau tab2 = zigzag1(matrice);
}
