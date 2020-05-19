#include "file.h"
#include "huffman.h"

int main(void) {
    file *f = creer_file();

    enfiler(f, Node_create(1));
    afficher_file(f);

    enfiler(f, Node_create(2));
    enfiler(f, Node_create(3));
    afficher_file(f);

    defiler(f);
    afficher_file(f);

    defiler(f);
    defiler(f);
    afficher_file(f);
    
    liberer_file(&f);
}