#include "file.h"
#include "huffman.h"

#include <stdio.h>

int main(void) {
    file *f = creer_file();

    enfiler(f, Node_create(1));
    afficher_file(f);

    enfiler(f, Node_create(2));
    enfiler(f, Node_create(3));
    afficher_file(f);

    Node *n1 = defiler(f);
    printf("Element défilé: %i\n", n1->symbol);
    Node_destroy(&n1);
    afficher_file(f);

    Node *n2 = defiler(f);
    printf("Element défilé: %i\n", n2->symbol);
    Node_destroy(&n2);
    Node *n3 = defiler(f);
    printf("Element défilé: %i\n", n3->symbol);
    Node_destroy(&n3);
    afficher_file(f);

    enfiler(f, Node_create(1));
    afficher_file(f);
    
    liberer_file(&f);
}