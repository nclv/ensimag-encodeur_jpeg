#include <stdio.h>

#include "htables.h"
#include "huffman.h"

int main(void) {
    /* On n'est pas obligé de mettre 16 éléments */
    uint8_t nb_symb_per_lengths[16] = {0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    uint8_t symbols[5] = {10, 11, 12, 13, 14};  // d, e, a, c, b
    uint8_t nb_symbols = 5;

    huff_table *ht = huffman_table_build(nb_symb_per_lengths, symbols, nb_symbols);

    afficher_huff_table(ht);

    huffman_table_destroy(ht);
}