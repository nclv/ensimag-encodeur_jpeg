#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <jpeg_writer.h>

/* Type opaque représentant un arbre de Huffman. */
struct huff_table;

/*
    Construit un arbre de Huffman à partir d'une table
    de symboles comme présenté en section 2.10.1 du sujet.
    nb_symb_per_lengths est un tableau contenant le nombre
    de symboles pour chaque longueur de 1 à 16,
    symbols est le tableau  des symboles ordonnés,
    et nb_symbols représente la taille du tableau symbols.
*/
extern struct huff_table *huffman_table_build(uint8_t *nb_symb_per_lengths,
                                              uint8_t *symbols,
                                              uint8_t nb_symbols);

/*
    Retourne le chemin dans l'arbre ht permettant d'atteindre
    la feuille de valeur value. nb_bits est un paramètre de sortie
    permettant de stocker la longueur du chemin retourné.
*/
extern uint32_t huffman_table_get_path(struct huff_table *ht,
                                       uint8_t value,
                                       uint8_t *nb_bits);

/*
   Retourne le tableau des symboles associé à l'arbre de
   Huffman passé en paramètre.
*/
extern uint8_t *huffman_table_get_symbols(struct huff_table *ht);

/*
    Retourne le tableau du nombre de symboles de chaque longueur
    associé à l'arbre de Huffman passé en paramètre.
*/
extern uint8_t *huffman_table_get_length_vector(struct huff_table *ht);

/*
    Détruit l'arbre de Huffman passé en paramètre et libère
    toute la mémoire qui lui est associée.
*/
extern void huffman_table_destroy(struct huff_table *ht);

#endif /* HUFFMAN_H */
