#include "huffman.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

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

huff_table *huffman_table_build(uint8_t *nb_symb_per_lengths, uint8_t *symbols, uint8_t nb_symbols) {
    huff_table *ht = malloc(sizeof *ht);
    if (ht == NULL) return NULL;

    ht->nb_symb_per_lengths = nb_symb_per_lengths;
    ht->symbols = symbols;
    ht->nb_symbols = nb_symbols;

    /* Initialisation des tailles. */
    ht->lengths = malloc(nb_symbols * sizeof(ht->lengths));
    if (ht->lengths == NULL) {
        free(ht);
        return NULL;
    }

    /* Construction du tableau des tailles à partir de nb_symb_per_lengths */
    uint8_t cumul = 0;
    for (uint8_t i = 0; i < 16; i++) {
        if (nb_symb_per_lengths[i] != 0) {
            for (uint8_t j = 0; j < nb_symb_per_lengths[i]; j++) {
                ht->lengths[j + cumul] = (uint8_t)(i + 1);
            }
            cumul = (uint8_t)(cumul + nb_symb_per_lengths[i]);
        }
        if (cumul == nb_symbols) break;
    }

    /* Initialisation des codes. */
    ht->codes = calloc(nb_symbols, sizeof(ht->codes));
    if (ht->codes == NULL) {
        free(ht->lengths);
        free(ht);
        return NULL;
    }

    /* Construction des codes canoniques: https://stackoverflow.com/a/34569068 */
    uint32_t code = 0;
    uint8_t count = 0;
    while (count < nb_symbols) {
        ht->codes[count] = code;
        code = (code + 1) << (ht->lengths[count + 1] - ht->lengths[count]);
        count++;
    }

    return ht;
}

uint32_t huffman_table_get_path(huff_table *ht, uint8_t value, uint8_t *nb_bits) {
    /* Si la valeur n'est pas dans la table on renvoie 65535 (16 * 1 en binaire )*/
    uint32_t code = 65535;
    /* Coût linéaire */
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