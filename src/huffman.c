#include "huffman.h"

#include <stdint.h>

#include "htables.h"

Node *Node_create(uint8_t symbol) {
    Node *node = malloc(sizeof *node);
    if (node == NULL) return NULL;

    node->left = NULL;
    node->right = NULL;

    node->symbol = symbol;

    return node;
}

huff_table *huffman_table_create(uint8_t *nb_symb_per_lengths, uint8_t *symbols, uint8_t nb_symbols) {
    huff_table *ht = malloc(sizeof *ht);
    if (ht == NULL) return NULL;

    ht->root = Node_create(NULL);
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

Node *fill_largeur(uint8_t *symbols, Node *root, uint8_t count, uint8_t nb_symbols, uint8_t *nb_symb_per_lengths, size_t i, uint8_t nb_symbols_level) {
    // test pour ne rajouter des noeuds qu'aux noeuds avec le champ symbol == NULL
    // surement mal placé
    if (count < nb_symbols && root->symbol != NULL) {
        nb_symbols_level += nb_symb_per_lengths[i];
        // noeud vide si étage rempli
        if (count > nb_symbols_level) {
            Node *temp = Node_create(NULL);
        } else {
            Node *temp = Node_create(symbols[count]);
        }
        root = temp;
        root->left = fill_largeur(symbols, root->left, 2 * count + 1, nb_symbols, nb_symb_per_lengths, i + 1, nb_symbols_level);
        root->right = fill_largeur(symbols, root->right, 2 * count + 2, nb_symbols, nb_symb_per_lengths, i + 1, nb_symbols_level);
    }
    return root;
}

huff_table *huffman_table_build(uint8_t *nb_symb_per_lengths, uint8_t *symbols, uint8_t nb_symbols) {
    huff_table *ht = huffman_table_create(nb_symb_per_lengths, symbols, nb_symbols);

    Node *root = fill_largeur(symbols, ht->root, 0, nb_symbols, nb_symb_per_lengths, 0, 0);
}

void huffman_table_destroy(huff_table *ht) {
    Node_destroy(&(ht->root));
    free(ht);
}