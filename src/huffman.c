#include "huffman.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "file.h"
#include "htables.h"

Node *Node_create(const uint32_t code) {
    Node *node = malloc(sizeof *node);
    if (node == NULL) return NULL;

    node->left = NULL;
    node->right = NULL;

    node->code = code;

    return node;
}

void Node_destroy(Node **node) {
    if ((*node) != NULL) {
        Node_destroy(&(*node)->left);
        Node_destroy(&(*node)->right);
        free(*node);
        *node = NULL;
    }
}

static void afficher_huffman_tree(const Node *root) {
    printf("%i \n", root->code);
    if (root->left) {
        afficher_huffman_tree(root->left);
    }
    if (root->right) {
        afficher_huffman_tree(root->right);
    }
}

void afficher_huff_table(huff_table *ht) {
    uint8_t *get_symbols = huffman_table_get_symbols(ht);
    for (size_t i = 0; i < ht->nb_symbols; i++) {
        printf("%i ", get_symbols[i]);
    }
    printf("\n");

    uint8_t *get_length = huffman_table_get_length_vector(ht);
    for (size_t i = 0; i < 16; i++) {
        printf("%i ", get_length[i]);
    }
    printf("\n");

    for (size_t i = 0; i < ht->nb_symbols; i++) {
        uint8_t nb_bits;
        uint32_t code = huffman_table_get_path(ht, ht->symbols[i], &nb_bits);
        printf("Code: %u, Length: %u\n", code, nb_bits);
    }
}

static huff_table *huffman_table_create(uint8_t *nb_symb_per_lengths, uint8_t *symbols, uint8_t nb_symbols) {
    huff_table *ht = malloc(sizeof *ht);
    if (ht == NULL) return NULL;

    ht->root = Node_create(0);
    if (ht->root == NULL) {
        free(ht);
        return NULL;
    }

    /* Initialisation des codes. */
    ht->codes = calloc(nb_symbols, sizeof ht->codes);
    if (ht->codes == NULL) {
        free(ht);
        return NULL;
    }
    /* Construction du tableau des tailles à partir de nb_symb_per_lengths */
    ht->lengths = malloc(nb_symbols * sizeof ht->lengths);
    if (ht->lengths == NULL) return NULL;
    uint8_t cumul = 0;
    for (uint8_t i = 0; i < 16; i++) {
        for (uint8_t j = 0; j < nb_symb_per_lengths[i]; j++) {
            ht->lengths[j + cumul] = (uint8_t)(i + 1);
        }
        cumul = (uint8_t)(cumul + nb_symb_per_lengths[i]);
    }

    ht->nb_symb_per_lengths = nb_symb_per_lengths;
    ht->symbols = symbols;
    ht->nb_symbols = nb_symbols;

    return ht;
}

static void huffman_tree_build(huff_table *ht, const uint8_t nb_symbols) {
    /* Allocation d'une file */
    file *f = creer_file();
    if (f == NULL) exit(EXIT_FAILURE);

    /* Root node and children */
    ht->root->left = Node_create(0);
    if (ht->root->left == NULL) exit(EXIT_FAILURE);
    ht->root->right = Node_create(1);
    if (ht->root->right == NULL) exit(EXIT_FAILURE);

    enfiler(f, ht->root->left);
    enfiler(f, ht->root->right);

    int level = 0;
    uint8_t nb_symbols_level_cumulee = ht->nb_symb_per_lengths[level];

    uint8_t count = 0;
    while (count < nb_symbols) {
        Node *node = defiler(f);
        if (node == NULL) continue;
        if (count == nb_symbols_level_cumulee) {
            /* On crè les 2 noeuds suivant s'il n'y a pas de symboles sur ce niveau */
            node->left = Node_create(node->code + (uint32_t)((1 << level) - 1));
            node->right = Node_create(node->code + (uint32_t)(1 << level));
            enfiler(f, node->left);
            enfiler(f, node->right);
            nb_symbols_level_cumulee = (uint8_t)(nb_symbols_level_cumulee + ht->nb_symb_per_lengths[level++]);
        } else {
            ht->codes[count] = node->code;
            count++;
        }
    }
    /* 
        Il reste un dernier élément dans la file car aucun code ne comporte que des 1.
        Il est libéré de l'arbre de Huffman avec la fonction Node_destroy.
    */
    Node *last = defiler(f);
    while (last != NULL) {
        // printf("Last %i\n", last->symbol);
        last = defiler(f);
    }
    liberer_file(&f);
}

huff_table *huffman_table_build(uint8_t *nb_symb_per_lengths, uint8_t *symbols, uint8_t nb_symbols) {
    huff_table *ht = huffman_table_create(nb_symb_per_lengths, symbols, nb_symbols);

    /* Construction de l'arbre */
    huffman_tree_build(ht, nb_symbols);
    afficher_huffman_tree(ht->root);

    /* On n'a plus besoin de l'arbre. */
    Node_destroy(&(ht->root));

    return ht;
}

uint32_t huffman_table_get_path(huff_table *ht, uint8_t value, uint8_t *nb_bits) {
    uint32_t code = 65535;
    /* Coût linéaire (une table de hashage c'est mieux !!) */
    for (size_t i = 0; i < ht->nb_symbols; i++) {
        if (ht->symbols[i] == value) {
            code = ht->codes[i];
            *nb_bits = ht->lengths[i];
            break;
        }
    }
    return code;
}

uint8_t *huffman_table_get_symbols(huff_table *ht) {
    return ht->symbols;
}

uint8_t *huffman_table_get_length_vector(huff_table *ht) {
    return ht->nb_symb_per_lengths;
}

void huffman_table_destroy(huff_table *ht) {
    free(ht->lengths);
    free(ht->codes);
    free(ht);
}