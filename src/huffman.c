#include "huffman.h"

#include <stdint.h>

#include "htables.h"

Node *Node_create(void) {
    Node *node = malloc(sizeof *node);
    if (node == NULL) return NULL;

    node->left = NULL;
    node->right = NULL;

    node->symbol = NULL;

    return node;
}

huff_table *huffman_table_create(uint8_t *nb_symb_per_lengths, uint8_t *symbols, uint8_t nb_symbols) {
    huff_table *ht = malloc(sizeof *ht);
    if (ht == NULL) return NULL;

    ht->root = Node_create();
    if (ht->root = NULL) {
        free(ht);
        return NULL;
    }

    ht->nb_symb_per_lengths = nb_symb_per_lengths;
    ht->symbols = symbols;
    ht->nb_symbols = nb_symbols;

    return ht;
}

void Node_destroy(Node **node) {
    if ((*node) != NULL) {
        Node_destroy((*node)->left);
        Node_destroy((*node)->right);
        free(*node);
        *node = NULL;
    }
}

huff_table *huffman_table_build(uint8_t *nb_symb_per_lengths, uint8_t *symbols, uint8_t nb_symbols) {
    huff_table *ht = huffman_table_create(nb_symb_per_lengths, symbols, nb_symbols);

    /* La longueur correspond aux différents niveaux de l'arbre */
    for (uint8_t longueur = 0; longueur < 16; longueur++) {
        for (uint8_t i = 0; i < nb_symb_per_lengths[longueur]; i += 2) {
            printf("");
        }
    }
}

void huffman_table_destroy(huff_table *ht) {
    Node_destroy(&(ht->root));
    free(ht);
}