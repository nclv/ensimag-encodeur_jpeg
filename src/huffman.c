#include "huffman.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "file.h"
#include "htables.h"

Node *Node_create(const uint8_t symbol) {
    Node *node = malloc(sizeof *node);
    if (node == NULL) return NULL;

    node->left = NULL;
    node->right = NULL;

    node->symbol = symbol;

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

void afficher_huffman_tree(const Node *root) {
    printf("%i \n", root->symbol);
    if (root->left) {
        afficher_huffman_tree(root->left);
    }
    if (root->right) {
        afficher_huffman_tree(root->right);
    }
}

static huff_table *huffman_table_create(uint8_t *nb_symb_per_lengths, uint8_t *symbols) {
    huff_table *ht = malloc(sizeof *ht);
    if (ht == NULL) return NULL;

    ht->root = Node_create(0);
    if (ht->root == NULL) {
        free(ht);
        return NULL;
    }

    ht->nb_symb_per_lengths = nb_symb_per_lengths;
    ht->symbols = symbols;

    return ht;
}

static void huffman_node_encode(const Node *node, uint8_t *code, uint8_t indice, uint32_t *codes) {
    /* Indice de remplissage de codes */
    static uint8_t index = 0;

    if (node->left != NULL) {
        code[indice] = 0;
        huffman_node_encode(node->left, code, (uint8_t)(indice + 1), codes);
    }
    if (node->right != NULL) {
        code[indice] = 1;
        huffman_node_encode(node->right, code, (uint8_t)(indice + 1), codes);
    }
    if ((node->left == NULL || node->right == NULL)) {
        /* Affichage des codes */
        // printf("%i: ", node->symbol);
        // for (size_t i = 0; i < indice; i++) {
        //     printf("%u", code[i]);
        // }
        // printf("\n");

        /* Encodage en uint32_t */
        int power = 0;
        codes[index] = 0;
        for (int j = indice - 1; j >= 0; j--) {
            codes[index] += (uint32_t)(code[j] << power);
            power += 1;
        }

        /* Code fonctionnel avec uint8_t **codes en paramètres */
        // codes[index] = malloc(indice * sizeof *codes[index]);
        // for (size_t j = 0; j < indice; j++) {
        //     codes[index][j] = code[j];
        // }

        /* Affichage des codes stockés */
        // for (size_t j = 0; j < indice; j++) {
        //     printf("%u", codes[index][j]);
        // }
        // printf("\n");

        index++;
    }
}

huff_table *huffman_table_build(uint8_t *nb_symb_per_lengths, uint8_t *symbols, uint8_t nb_symbols) {
    huff_table *ht = huffman_table_create(nb_symb_per_lengths, symbols);

    /* Allocation d'une file */
    file *f = creer_file();
    if (f == NULL) return NULL;

    /* Root node and children */
    ht->root->left = Node_create(0);
    if (ht->root->left == NULL) return NULL;
    ht->root->right = Node_create(1);
    if (ht->root->right == NULL) return NULL;

    enfiler(f, ht->root->left);
    enfiler(f, ht->root->right);

    int level = 0;
    uint8_t nb_symbols_level_cumulee = nb_symb_per_lengths[level];

    uint8_t count = 0;
    while (count < nb_symbols) {
        Node *node = defiler(f);
        if (node == NULL) continue;
        if (count == nb_symbols_level_cumulee) {
            /* On crè les 2 noeuds vide suivant s'il n'y a pas de symboles sur ce niveau */
            node->left = Node_create(0);
            node->right = Node_create(1);
            enfiler(f, node->left);
            enfiler(f, node->right);
            nb_symbols_level_cumulee = (uint8_t)(nb_symbols_level_cumulee + nb_symb_per_lengths[level++]);
        } else {
            /* On assigne un symbole au noeud */
            node->symbol = symbols[count++];
        }
    }
    /* 
        Il reste un dernier élément dans la file car aucun code ne comporte que des 1.
        Il est libéré avec l'arbre de Huffman avec la fonction Node_destroy.
    */
    Node *last = defiler(f);
    while (last != NULL) {
        // printf("Last %i\n", last->symbol);
        last = defiler(f);
    }
    liberer_file(&f);

    /* Code de longueur maximale 16 */
    uint8_t code[16] = {0};
    uint32_t codes[nb_symbols];

    huffman_node_encode(ht->root, code, 0, codes);

    /* Construction du tableau des tailles */
    // uint8_t lengths[nb_symbols];
    // uint8_t cumul = 0;
    // for (uint8_t i = 0; i < 16; i++) {
    //     for (uint8_t j = 0; j < nb_symb_per_lengths[i]; j++) {
    //         lengths[j + cumul] = (uint8_t)(i + 1);
    //     }
    //     cumul = (uint8_t)(cumul + nb_symb_per_lengths[i]);
    // }

    for (size_t j = 0; j < nb_symbols; j++) {
        printf("%u\n", codes[j]);
    }

    /* Affichage des codes */
    // for (size_t j = 0; j < nb_symbols; j++) {
    //     for (size_t i = 0; i < lengths[j]; i++) {
    //         printf("%u", codes[j][i]);
    //     }
    //     printf("\n");
    // }

    return ht;
}

// uint32_t huffman_table_get_path(huff_table *ht, uint8_t value, uint8_t *nb_bits) {}

uint8_t *huffman_table_get_symbols(huff_table *ht) {
    return ht->symbols;
}

uint8_t *huffman_table_get_length_vector(huff_table *ht) {
    return ht->nb_symb_per_lengths;
}

void huffman_table_destroy(huff_table *ht) {
    Node_destroy(&(ht->root));
    free(ht);
}