#ifndef _BITSTREAM_H_
#define _BITSTREAM_H_

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#define DEFAULT_BUFFER_SIZE 2048  // valeur à modifier, 1 pour les tests

/*
 * Type opaque représentant le flux d'octets à écrire dans le fichier JPEG de
 * sortie (appelé bitstream dans le sujet).
 * La création et la fermeture du bitstream est faite dans les fonctions
 * jpeg_write_header et jpeg_desc_destroy du module jpeg_writer.
 */
// #pragma pack(1)
typedef struct bitstream {
    FILE *file; /* Ecriture effective lorsque le buffer est rempli */

    unsigned char bytes_buffer[DEFAULT_BUFFER_SIZE]; /* Données à écrire */
    size_t bytes_buffer_size;
    size_t last_written_byte_offset; /* Position du dernier byte écrit */

    unsigned char bits_buffer;
    size_t bits_buffer_size;        /* nombre de bits écrits dans bits_buffer */
    size_t last_written_bit_offset; /* Position du dernier bit écrit */

} bitstream;

/* type : char*
 * rtype : bitstream*
 * Retourne un nouveau bitstream prêt à écrire dans le fichier filename initialement vide.
 * Cette fonction alloue dynamiquement une variable de type struct bitstream, la remplit de
 * manière adéquate, puis retourne son adresse mémoire. En cas d'erreur, la fonction retourne
 * NULL.
 * La mémoire allouée doit ensuite être libérée par bitstream_destroy.
 */
extern struct bitstream *bitstream_create(const char *filename);

/* type : bistream*
 * type : uint32_t
 * type : uint8_t
 * type : bool
 * rtype : void
 * Ecrit nb_bits bits dans le bitstream.
 * La valeur portée par cet ensemble de bits est value.
 * Le paramètre is_marker permet d'indiquer qu'on est en train
 * d'écrire un marqueur de section dans l'entête JPEG ou non (voir section
 * 2.10.4 du sujet).
*/
extern void bitstream_write_bits(struct bitstream *stream,
                                 uint32_t value,
                                 uint8_t nb_bits,
                                 bool is_marker);

/* type : bitstream*
 * rtype : void
 * fonction d'affichage
 */
extern void bitstream_display(bitstream *stream);

/* type : bitstream*
 * rtype : void
 * Force l'exécution des écritures en attente sur le bitstream, s'il en
 * existe.
 */
extern void bitstream_flush(struct bitstream *stream);

/* type : bitstream*
 * rtype : void
 * Détruit le bitstream passé en paramètre, en libérant la mémoire qui lui est
 * associée.
 */
extern void bitstream_destroy(struct bitstream *stream);

#endif /* _BITSTREAM_H_ */
