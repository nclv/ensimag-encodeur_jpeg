#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <stdint.h>

/* Type opaque représentant un arbre de Huffman. */
// #pragma pack(1)
typedef struct huff_table {
    uint8_t *symbols;
    uint8_t *nb_symb_per_lengths;
    uint8_t nb_symbols;
    uint8_t *lengths;
    uint32_t *codes;
} huff_table;

/* type : huff_table*
 * rtype : void
 * fonction d'affichage
 */
extern void afficher_huff_table(huff_table *ht);

/* type : uint8_t*
 * type : uint8_t *
 * type : uint8_t
 * rtype : huff_table*
 * Construit un arbre de Huffman à partir d'une table de symboles comme
 * présenté en section 2.10.1 du sujet.
 * nb_symb_per_lengths est un tableau contenant le nombre
 * de symboles pour chaque longueur de 1 à 16,
 * symbols est le tableau des symboles ordonnés,
 * et nb_symbols représente la taille du tableau symbols.
 */
extern huff_table *huffman_table_build(uint8_t *nb_symb_per_lengths,
                                       uint8_t *symbols,
                                       uint8_t nb_symbols);

/* type : huff_table*
 * type : uint8_t
 * type : uint8_t*
 * rtype : uint32_t
 * Retourne le chemin dans l'arbre ht permettant d'atteindre
 * la feuille de valeur value. nb_bits est un paramètre de sortie
 * permettant de stocker la longueur du chemin retourné.
 */
extern uint32_t huffman_table_get_path(huff_table *ht,
                                       uint8_t value,
                                       uint8_t *nb_bits);

/* type : huff_table*
 * rtype : uint8_t
 * Retourne le tableau des symboles associé à l'arbre de
 * Huffman passé en paramètre.
 */
extern uint8_t *huffman_table_get_symbols(huff_table *ht);

/* type : huff_table*
 * rtype : uint8_t*
 * Retourne le tableau du nombre de symboles de chaque longueur
 * associé à l'arbre de Huffman passé en paramètre.
 */
extern uint8_t *huffman_table_get_length_vector(huff_table *ht);

/* type : huff_table*
 * rtype : void
 * Détruit l'arbre de Huffman passé en paramètre et libère
 * toute la mémoire qui lui est associée.
 */
extern void huffman_table_destroy(huff_table *ht);

#endif /* HUFFMAN_H */
