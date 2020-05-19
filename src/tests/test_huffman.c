#include "huffman.h"

#include <stdio.h>

int main(void) {
    /* On n'est pas obligé de mettre 16 éléments */
    uint8_t nb_symb_per_lengths[16] = {0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    uint8_t symbols[5] = {10, 11, 12, 13, 14}; // d, e, a, c, b
    uint8_t nb_symbols = 5;

    huff_table *ht = huffman_table_build(nb_symb_per_lengths, symbols, nb_symbols);

    afficher_huffman_tree(ht->root);

    uint8_t *get_symbols = huffman_table_get_symbols(ht);
    for (size_t i = 0; i < 5; i++) {
        printf("%i ", get_symbols[i]);
    }
    printf("\n");

    uint8_t *get_length = huffman_table_get_length_vector(ht);
    for (size_t i = 0; i < 16; i++) {
        printf("%i ", get_length[i]);
    }
    printf("\n");

    huffman_table_destroy(ht);
}