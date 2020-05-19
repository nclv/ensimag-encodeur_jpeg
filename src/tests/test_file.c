#include "file.h"
#include "huffman.h"

#include <stdio.h>

int main(void) {
    file *f = creer_file();

    /* 
        Il est plus prudent d'initialiser le noeud avant d'enfiler 
        car la file ne s'occupe pas de libérer les noeuds.

        L'initialisation directement en paramètre ne pose pas de 
        problème si le noeud est ensuite dépilé et libéré.
    */
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

    Node *n4 = Node_create(1);
    enfiler(f, n4);
    afficher_file(f);
    
    liberer_file(&f);
    Node_destroy(&n4);
}