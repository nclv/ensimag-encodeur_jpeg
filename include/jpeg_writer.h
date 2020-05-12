#ifndef JPEG_WRITER_H
#define JPEG_WRITER_H

#include <stdint.h>

/********************/
/* Types de données */
/********************/

/* Type énuméré représentant les composantes de couleur YCbCr. */
enum color_component
{
    Y,
    Cb,
    Cr,
    NB_COLOR_COMPONENTS
};

/*
    Type énuméré représentant les types de composantes fréquentielles (DC ou AC).
*/
enum sample_type
{
    DC,
    AC,
    NB_SAMPLE_TYPES
};

/*
    Type énuméré représentant la direction des facteurs d'échantillonnage (H
    pour horizontal, V pour vertical).
*/
enum direction
{
    H,
    V,
    NB_DIRECTIONS
};

/*
    Type opaque contenant l'intégralité des informations 
    nécessaires à l'écriture de l'en-tête JPEG.
*/
struct jpeg;

/* Type opaque représentant un arbre de Huffman. */
struct huff_table;


/***********************************************/
/* Ouverture, fermeture et fonctions générales */
/***********************************************/

/* Alloue et retourne une nouvelle structure jpeg. */
extern struct jpeg *jpeg_create(void);

/*
    Détruit une structure jpeg. 
    Toute la mémoire qui lui est associée est libérée.
*/
extern void jpeg_destroy(struct jpeg *jpg);

/*
    Ecrit tout l'en-tête JPEG dans le fichier de sortie à partir des
    informations contenues dans la structure jpeg passée en paramètre. 
    En sortie, le bitstream est positionné juste après l'écriture de 
    l'en-tête SOS, à l'emplacement du premier octet de données brutes à écrire.
*/
extern void jpeg_write_header(struct jpeg *jpg);

/* Ecrit le footer JPEG (marqueur EOI) dans le fichier de sortie. */
extern void jpeg_write_footer(struct jpeg *jpg);

/*
    Retourne le bitstream associé au fichier de sortie enregistré 
    dans la structure jpeg.
*/
extern struct bitstream *jpeg_get_bitstream(struct jpeg *jpg);


/****************************************************/
/* Gestion des paramètres de l'encodeur via le jpeg */
/****************************************************/

/* Ecrit le nom de fichier PPM ppm_filename dans la structure jpeg. */
extern void jpeg_set_ppm_filename(struct jpeg *jpg,
                                  const char *ppm_filename);

/* Retourne le nom de fichier PPM lu dans la structure jpeg. */
extern char *jpeg_get_ppm_filename(struct jpeg *jpg);

/* Ecrit le nom du fichier de sortie jpeg_filename dans la structure jpeg. */
extern void jpeg_set_jpeg_filename(struct jpeg *jpg,
                                   const char *jpeg_filename);

/* Retourne le nom du fichier de sortie lu depuis la structure jpeg. */
extern char *jpeg_get_jpeg_filename(struct jpeg *jpg);


/*
    Ecrit la hauteur de l'image traitée, en nombre de pixels,
    dans la structure jpeg.
*/
extern void jpeg_set_image_height(struct jpeg *jpg,
                                  uint32_t image_height);

/*
    Retourne la hauteur de l'image traitée, en nombre de pixels,
    lue dans la structure jpeg.
*/
extern uint32_t jpeg_get_image_height(struct jpeg *jpg);

/*
    Ecrit la largeur de l'image traitée, en nombre de pixels,
    dans la structure jpeg.
*/
extern void jpeg_set_image_width(struct jpeg *jpg,
                                 uint32_t image_width);

/*
    Retourne la largeur de l'image traitée, en nombre de pixels,
    lue dans la structure jpeg.
*/
extern uint32_t jpeg_get_image_width(struct jpeg *jpg);


/*
    Ecrit le nombre de composantes de couleur de l'image traitée
    dans la structure jpeg.
*/
extern void jpeg_set_nb_components(struct jpeg *jpg,
                                   uint8_t nb_components);

/*
    Retourne le nombre de composantes de couleur de l'image traitée 
    lu dans la structure jpeg.
*/
extern uint8_t jpeg_get_nb_components(struct jpeg *jpg);


/*
    Ecrit dans la structure jpeg le facteur d'échantillonnage sampling_factor
    à utiliser pour la composante de couleur cc et la direction dir.
*/
extern void jpeg_set_sampling_factor(struct jpeg *jpg,
                                     enum color_component cc,
                                     enum direction dir,
                                     uint8_t sampling_factor);

/*
    Retourne le facteur d'échantillonnage utilisé pour la composante 
    de couleur cc et la direction dir, lu dans la structure jpeg.
*/
extern uint8_t jpeg_get_sampling_factor(struct jpeg *jpg,
                                        enum color_component cc,
                                        enum direction dir);


/*
    Ecrit dans la structure jpeg la table de Huffman huff_table à utiliser
    pour encoder les données de la composante fréquentielle acdc, pour la
    composante de couleur cc.
*/
extern void jpeg_set_huffman_table(struct jpeg *jpg,
                                   enum sample_type acdc,
                                   enum color_component cc,
                                   struct huff_table *htable);

/*
    Retourne un pointeur vers la table de Huffman utilisée pour encoder
    les données de la composante fréquentielle acdc pour la composante 
    de couleur cc, lue dans la structure jpeg.
*/
extern struct huff_table *jpeg_get_huffman_table(struct jpeg *jpg,
                                                 enum sample_type acdc,
                                                 enum color_component cc);


/*
    Ecrit dans la structure jpeg la table de quantification à utiliser
    pour compresser les coefficients de la composante de couleur cc.
*/
extern void jpeg_set_quantization_table(struct jpeg *jpg,
                                        enum color_component cc,
                                        uint8_t *qtable);

/*
    Retourne un pointeur vers la table de quantification associée à la 
    composante de couleur cc, lue dans a structure jpeg.
*/
extern uint8_t *jpeg_get_quantization_table(struct jpeg *jpg,
                                            enum color_component cc);

#endif /* JPEG_WRITER_H */
