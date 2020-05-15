#include "../include/zigzag.h"

int main(void){
  // Matrice 8x8 de nombres aléatoirs entre 0 et 255
  bloc matrice;
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      matrice.bloc[i][j] = rand() % 255;
      printf("%u", matrice.bloc[i][j]);
    }
  }

  // zigzag de la matrice
  tableau tab = zigzag1(matrice);
  //uint32_t* tab2 = zigzag2(matrice);

}
