#ifndef JPEG_WRITER_H
#define JPEG_WRITER_H

#include <stdint.h>

#include "bitstream.h"
#include "huffman.h"

/********************/
/* Types de données */
/********************/

/* Type énuméré représentant les composantes de couleur YCbCr. */
enum color_component {
    Y,
    Cb,
    Cr,
    NB_COLOR_COMPONENTS
};

/* Type énuméré représentant les types de composantes
 * fréquentielles (DC ou AC).
 */
enum sample_type {
    DC,
    AC,
    NB_SAMPLE_TYPES
};

/* Type énuméré représentant la direction des facteurs d'échantillonnage
 * (H pour horizontal, V pour vertical).
 */
enum direction {
    H,
    V,
    NB_DIRECTIONS
};

/* Type opaque contenant l'intégralité des informations
 * nécessaires à l'écriture de l'en-tête JPEG.
 *
 * Nécessaires :
 * filename,
 */
typedef struct jpeg {
    const char *jpeg_filename;
    const char *input_filename;

    uint32_t image_height;
    uint32_t image_width;
    uint8_t nb_components;
    bitstream *stream;

    /* 4 bits : Facteur d'échantillonnage (sampling factor) horizontal, de 1 à 4
     * 4 bits : Facteur d'échantillonnage (sampling factor) vertical, de 1 à 4
    */
    uint8_t sampling_factors[NB_COLOR_COMPONENTS][NB_DIRECTIONS];  // ou encore [3][2]

    huff_table *ht_Y_DC;
    huff_table *ht_Y_AC;
    huff_table *ht_CbCr_DC;
    huff_table *ht_CbCr_AC;

    uint8_t *Y_quantization_table;
    uint8_t *CbCr_quantization_table;
} jpeg;

/***********************************************/
/* Ouverture, fermeture et fonctions générales */
/***********************************************/

/* type : void
 * rtype : jpeg*
 * Alloue et retourne une nouvelle structure jpeg.
 */
extern jpeg *jpeg_create(void);

/* type : jpeg*
 * rtype : void
 * Détruit une structure jpeg.
 * Toute la mémoire qui lui est associée est libérée.
 */
extern void jpeg_destroy(jpeg *jpg);

/* type : jpeg*
 * rtype : void
 * Ecrit tout l'en-tête JPEG dans le fichier de sortie à partir des
 * informations contenues dans la structure jpeg passée en paramètre.
 * En sortie, le bitstream est positionné juste après l'écriture de
 * l'en-tête SOS, à l'emplacement du premier octet de données brutes à écrire.
 */
extern void jpeg_write_header(jpeg *jpg);

/* type : jpeg*
 * rtype : void
 * Ecrit le footer JPEG (marqueur EOI) dans le fichier de sortie.
 */
extern void jpeg_write_footer(jpeg *jpg);

/* type : jpeg*
 * rtype : bitstream*
 * Retourne le bitstream associé au fichier de sortie enregistré
 * dans la structure jpeg.
 */
extern bitstream *jpeg_get_bitstream(jpeg *jpg);

/****************************************************/
/* Gestion des paramètres de l'encodeur via le jpeg */
/****************************************************/

/* type : jpeg*
 * type : const char*
 * rtype : void
 * Ecrit le nom de fichier PPM/PGM input_filename dans la structure jpeg.
 */
extern void jpeg_set_ppm_filename(jpeg *jpg,
                                  const char *input_filename);

/* type : jpeg*
 * rtype char*
 * Retourne le nom de fichier PPM/PGM lu dans la structure jpeg.
 */
extern char *jpeg_get_ppm_filename(jpeg *jpg);

/* type : jpeg*
 * type : const char*
 * Ecrit le nom du fichier de sortie jpeg_filename dans la structure jpeg.
 */
extern void jpeg_set_jpeg_filename(jpeg *jpg,
                                   const char *jpeg_filename);

/* type : jepg*
 * rtype char*
 * Retourne le nom du fichier de sortie lu depuis la structure jpeg.
 */
extern char *jpeg_get_jpeg_filename(jpeg *jpg);

/* type : jpeg*
 * type : uint32_t
 * Ecrit la hauteur de l'image traitée, en nombre de pixels,
 * dans la structure jpeg.
 */
extern void jpeg_set_image_height(jpeg *jpg,
                                  uint32_t image_height);

/* type : jpeg*
 * rtype : uint32_t
 * Retourne la hauteur de l'image traitée, en nombre de pixels,
 * lue dans la structure jpeg.
 */
extern uint32_t jpeg_get_image_height(jpeg *jpg);

/* type : jpeg*
 * type : uint32_t
 * rtype : void
 * Ecrit la largeur de l'image traitée, en nombre de pixels,
 * dans la structure jpeg.
 */
extern void jpeg_set_image_width(jpeg *jpg,
                                 uint32_t image_width);

/* type : jpeg*
 * rtype : uint32_t
 * Retourne la largeur de l'image traitée, en nombre de pixels,
 * lue dans la structure jpeg.
 */
extern uint32_t jpeg_get_image_width(jpeg *jpg);

/* type : jpeg*
 * type : uint8_t
 * Ecrit le nombre de composantes de couleur de l'image traitée
 * dans la structure jpeg.
 */
extern void jpeg_set_nb_components(jpeg *jpg,
                                   uint8_t nb_components);

/* type : jpeg*
 * rtype : uint8_t
 * Retourne le nombre de composantes de couleur de l'image traitée
 * lu dans la structure jpeg.
 */
extern uint8_t jpeg_get_nb_components(jpeg *jpg);

/* type : jpeg*
 * type : color_component
 * type : direction
 * type : uint8_t
 * rtype : void
 * Ecrit dans la structure jpeg le facteur d'échantillonnage sampling_factor
 * à utiliser pour la composante de couleur cc et la direction dir.
 */
extern void jpeg_set_sampling_factor(jpeg *jpg,
                                     enum color_component cc,
                                     enum direction dir,
                                     uint8_t sampling_factor);

/* type : jpeg
 * type : color_component
 * type : direction
 * Retourne le facteur d'échantillonnage utilisé pour la composante
 * de couleur cc et la direction dir, lu dans la structure jpeg.
 */
extern uint8_t jpeg_get_sampling_factor(jpeg *jpg,
                                        enum color_component cc,
                                        enum direction dir);

/* type : jpeg*
 * type : sample_type
 * type : color_component
 * type huff_table*
 * rtype : void
 * Ecrit dans la structure jpeg la table de Huffman huff_table à utiliser
 * pour encoder les données de la composante fréquentielle acdc, pour la
 * composante de couleur cc.
 */
extern void jpeg_set_huffman_table(jpeg *jpg,
                                   enum sample_type acdc,
                                   enum color_component cc,
                                   huff_table *htable);

/* type : jpeg*
 * type : sample_type
 * color_component
 * rtype : huff_table*
 * Retourne un pointeur vers la table de Huffman utilisée pour encoder
 * les données de la composante fréquentielle acdc pour la composante
 * de couleur cc, lue dans la structure jpeg.
 */
extern huff_table *jpeg_get_huffman_table(jpeg *jpg,
                                          enum sample_type acdc,
                                          enum color_component cc);

/* type : jpeg*
 * type color_compnent
 * type : uint8_t
 * Ecrit dans la structure jpeg la table de quantification à utiliser
 * pour compresser les coefficients de la composante de couleur cc.
 */
extern void jpeg_set_quantization_table(jpeg *jpg,
                                        enum color_component cc,
                                        uint8_t *qtable);

/* type : jpeg*
 * type : color_component
 * Retourne un pointeur vers la table de quantification associée à la
 * composante de couleur cc, lue dans a structure jpeg.
 */
extern uint8_t *jpeg_get_quantization_table(jpeg *jpg,
                                            enum color_component cc);

#endif /* JPEG_WRITER_H */
