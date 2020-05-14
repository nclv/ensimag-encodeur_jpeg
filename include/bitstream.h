#ifndef _BITSTREAM_H_
#define _BITSTREAM_H_

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/*
    Type opaque représentant le flux d'octets à écrire dans le fichier JPEG de
    sortie (appelé bitstream dans le sujet).

    La création et la fermeture du bitstream est faite dans les fonctions
    jpeg_write_header et jpeg_desc_destroy du module jpeg_writer.
*/
typedef struct bitstream {
    const char* filename; /* Ecriture effective lorsque le buffer est rempli */
    unsigned char *buffer; /* Données à écrire */
    size_t buffer_size;
    size_t last_written_bit_offset; /* Position du dernier bit écrit dans le bitstream */
} bitstream;

/* 
    Retourne un nouveau bitstream prêt à écrire dans le fichier filename initialement vide. 
    Cette fonction alloue dynamiquement une variable de type struct bitstream, la remplit de 
    manière adéquate, puis retourne son adresse mémoire. En cas d'erreur, la fonction retourne 
    NULL.
    La mémoire allouée doit ensuite être libérée par bitstream_destroy.
*/
extern struct bitstream *bitstream_create(const char *filename);

/*
    Ecrit nb_bits bits dans le bitstream. La valeur portée par cet ensemble de
    bits est value. Le paramètre is_marker permet d'indiquer qu'on est en train
    d'écrire un marqueur de section dans l'entête JPEG ou non (voir section
    2.10.4 du sujet).
*/
extern void bitstream_write_bits(struct bitstream *stream,
                                 uint32_t value,
                                 uint8_t nb_bits,
                                 bool is_marker);

extern void bitstream_display(bitstream *stream);

/*
    Force l'exécution des écritures en attente sur le bitstream, s'il en
    existe.
*/
extern void bitstream_flush(struct bitstream *stream);

/*
    Détruit le bitstream passé en paramètre, en libérant la mémoire qui lui est
    associée.
*/
extern void bitstream_destroy(struct bitstream *stream);

#endif /* _BITSTREAM_H_ */
