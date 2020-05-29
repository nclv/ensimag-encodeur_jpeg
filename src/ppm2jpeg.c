#include "ppm2jpeg.h"

#include <argp.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "dct.h"
#include "encode_ACDC.h"
#include "htables.h"
#include "jpeg_writer.h"
#include "ppm.h"
#include "qtables.h"
#include "quantification.h"
#include "utils.h"
#include "zigzag.h"

/*Parsing des options*/
static error_t parse_option(int key, char *arg, struct argp_state *state) {
    arguments *args = state->input;

    switch (key) {
        case ARGP_KEY_ARG:
            (state->arg_num == 0) ? args->inputfile = arg : argp_usage(state);
            break;
        case 'o':
            args->outfile = arg;
            break;
        case 's':
            args->sample = arg;
            break;
        case ARGP_KEY_END:
            if (args->inputfile == NULL) argp_usage(state);
            break;
        default:
            return ARGP_ERR_UNKNOWN;
    }

    return 0;
}

static void afficher_options(const arguments *args, const char *outfile, const uint8_t sampling_factors[NB_COLOR_COMPONENTS][NB_DIRECTIONS], const bool no_downsampling) {
    printf("input file: %s\noutput file: %s\n", args->inputfile, outfile);
    printf("No downsampling: ");
    printf(no_downsampling ? "true\n" : "false\n");
    for (size_t cc = Y; cc < NB_COLOR_COMPONENTS; cc++) {
        for (size_t dir = H; dir < NB_DIRECTIONS; dir++) {
            printf("%d ", sampling_factors[cc][dir]);
        }
    }
    printf("\n");
}

/* Vérifie l'extension de filename */
bool check_extension(const char *filename, const char *extension) {
    return !strcmp(filename, extension);
}

/*Renvoie True si le caractère c est un digit*/
bool is_digit(const char c) {
    return c >= '0' && c <= '9';
}

/*Vérifie la syntaxe des arguments donnés*/
bool verifier_sample(const char *sample) {
    return strlen(sample) == 11 &&
           is_digit(sample[0]) &&
           sample[1] == 'x' &&
           is_digit(sample[2]) &&
           sample[3] == ',' &&
           is_digit(sample[4]) &&
           sample[5] == 'x' &&
           is_digit(sample[6]) &&
           sample[7] == ',' &&
           is_digit(sample[8]) &&
           sample[9] == 'x' &&
           is_digit(sample[10]);
}

/*Vérifie les spécifications de la norme JPEG*/
bool verifier_facteurs(uint8_t facteurs[NB_COLOR_COMPONENTS][NB_DIRECTIONS]) {
    bool est_correct = true;
    for (size_t cc = Y; cc < NB_COLOR_COMPONENTS; cc++) {
        for (size_t dir = H; dir < NB_DIRECTIONS; dir++) {
            est_correct = est_correct && facteurs[cc][dir] >= 1 && facteurs[cc][dir] <= 4;
        }
    }

    est_correct = est_correct && (facteurs[Y][H] * facteurs[H][V] + facteurs[Cb][H] * facteurs[Cb][V] + facteurs[Cr][H] * facteurs[Cr][V] <= 10);
    est_correct = est_correct && (facteurs[Y][H] % facteurs[Cb][H] == 0 && facteurs[Y][H] % facteurs[Cr][H] == 0 && facteurs[H][V] % facteurs[Cb][V] == 0 && facteurs[H][V] % facteurs[Cr][V] == 0);

    return est_correct;
}

void verifier_syntaxe(arguments args) {
    // if (!(check_one_dot(args.inputfile)) || (strlen(args.outfile) != 0 && !(check_one_dot(args.outfile)))) {
    //     printf("ERREUR: Trop ou pas assez de points dans les noms de fichiers IO\n");
    //     exit(EXIT_FAILURE);
    // }
    if (!(check_extension(strrchr(args.inputfile, '.'), ".ppm") || check_extension(strrchr(args.inputfile, '.'), ".pgm"))) {
        printf("ERREUR: Les fichiers d'entrée ne sont pas reconnus\n");
        exit(EXIT_FAILURE);
    }
    if (strlen(args.outfile) != 0 && !check_extension(strrchr(args.outfile, '.'), ".jpg")) {
        printf("ERREUR: Le fichier de sortie n'est pas reconnu\n");
        exit(EXIT_FAILURE);
    }
    // if (check_extension(strchr(args.inputfile, '.'), ".ppm") && !strcmp(args.sample, "")) {
    //     printf("ERREUR: Les facteurs d'échantillonage ne sont pas spécifiés.\n");
    //     exit(EXIT_FAILURE);
    // }
    if (strlen(args.sample) != 0 && !verifier_sample(args.sample)) {
        printf("ERREUR: Le format des facteurs d'échantillonage est incorrect\n");
        exit(EXIT_FAILURE);
    }
}

static void encode_data_unit(int16_t **data_unit, int16_t data_unit_freq[TAILLE_DATA_UNIT][TAILLE_DATA_UNIT], uint8_t qtable[64]) {
    printf("\nENCODAGE \n");
    offset(data_unit);
    dct(data_unit, data_unit_freq);
    afficher_dct(data_unit_freq);

    zigzag_inplace(data_unit_freq);
    afficher_zigzag(data_unit_freq);

    quantifier(data_unit_freq, qtable);
    afficher_matrice_quantifiee(data_unit_freq);
}

static int16_t process_Y(int16_t **Y_mcu, uint8_t hy, uint8_t vy,
                         int16_t **data_unit, int16_t data_unit_freq[8][8],
                         huff_table *Y_dc_table, huff_table *Y_ac_table,
                         bitstream *stream, int16_t difference_DC_Y) {
    printf("Traitement des blocs Y\n");
    // printf("%d %d\n", hy, vy);
    for (size_t v = 0; v < vy; v++) {
        for (size_t h = 0; h < hy; h++) {
            for (size_t i = 0; i < 8; i++) {
                for (size_t j = 0; j < 8; j++) {
                    data_unit[i][j] = Y_mcu[i + 8 * v][j + 8 * h];
                }
            }
            /* Encodage de data_unit */
            encode_data_unit(data_unit, data_unit_freq, quantification_table_Y);
            ecrire_coeffs(stream, data_unit_freq, Y_dc_table, Y_ac_table, difference_DC_Y);
            difference_DC_Y = data_unit_freq[0][0];
        }
    }
    return difference_DC_Y;
}

static int16_t process_chroma(int16_t **chroma_mcu, uint8_t hy, uint8_t vy,
                              uint8_t h_chroma, uint8_t v_chroma,
                              int16_t **data_unit, int16_t data_unit_freq[8][8],
                              huff_table *CbCr_dc_table, huff_table *CbCr_ac_table,
                              bitstream *stream, int16_t difference_DC_chroma) {
    printf("Traitement des blocs Cb / Cr avec échantillonnage horizontal");
    // printf("vy: %d \n", vy);
    // printf("h_chroma, v_chroma: %d %d\n", h_chroma, v_chroma);
    if (h_chroma == 0 || v_chroma == 0) {
        fprintf(stderr, "Exception en point flottant: Un des arguments h_chroma / v_chroma est nul.");
        exit(EXIT_FAILURE);
    }
    float remainder = 0;
    uint8_t chroma_value;
    uint8_t h_div = (hy / h_chroma);
    uint8_t v_div = (vy / v_chroma);
    // printf("%d %d\n", h_div, v_div);
    /* On boucle dans l'ordre des DU à traiter */
    for (size_t v = 0; v < v_chroma; v++) {
        for (size_t h = 0; h < h_chroma; h++) {
            // printf("%ld, %ld\n", v, h);
            /* Echantillonnage de la chrominance */
            for (size_t i = 0; i < 8; i++) {
                for (size_t j = 0; j < 8; j++) {
                    for (size_t k = 0; k < h_div; k++) {
                        for (size_t l = 0; l < v_div; l++) {
                            chroma_value = (uint8_t)chroma_mcu[v_div * i + l + 8 * v][h_div * j + k + 8 * h];
                            data_unit[i][j] = (uint8_t)(data_unit[i][j] + chroma_value / (h_div * v_div));
                            remainder += (float)(chroma_value % (h_div * v_div));
                        }
                    }
                    data_unit[i][j] = (int16_t)(data_unit[i][j] + remainder / (float)(h_div * v_div));
                    remainder = 0;
                }
            }
            /* Traitement des DUs */
            // afficher_data_unit(data_unit);
            encode_data_unit(data_unit, data_unit_freq, quantification_table_CbCr);
            ecrire_coeffs(stream, data_unit_freq, CbCr_dc_table, CbCr_ac_table, difference_DC_chroma);
            difference_DC_chroma = data_unit_freq[0][0];
            /* Mise à zéro de data_unit */
            for (size_t i = 0; i < 8; i++) {
                for (size_t j = 0; j < 8; j++) {
                    data_unit[i][j] = 0;
                }
            }
        }
    }
    return difference_DC_chroma;
}

static void jpeg_set_tables_Y(jpeg *jpg) {
    printf("\nEcriture de la table de quantification de la composante Y\n");
    jpeg_set_quantization_table(jpg, Y, quantification_table_Y);

    printf("\nConstruction des tables de Huffman de la composante Y\n");
    huff_table *htable_Y_DC = huffman_table_build(htables_nb_symb_per_lengths[DC][Y], htables_symbols[DC][Y], htables_nb_symbols[DC][Y]);
    if (htable_Y_DC == NULL) {
        jpeg_destroy(jpg);
        exit(EXIT_FAILURE);
    }
    jpeg_set_huffman_table(jpg, DC, Y, htable_Y_DC);

    huff_table *htable_Y_AC = huffman_table_build(htables_nb_symb_per_lengths[AC][Y], htables_symbols[AC][Y], htables_nb_symbols[AC][Y]);
    if (htable_Y_AC == NULL) {
        jpeg_destroy(jpg);
        exit(EXIT_FAILURE);
    }
    jpeg_set_huffman_table(jpg, AC, Y, htable_Y_AC);
}

static void jpeg_set_tables_CbCr(jpeg *jpg) {
    printf("\nEcriture de la table de quantification des composantes CbCr Y\n");
    jpeg_set_quantization_table(jpg, Cb, quantification_table_CbCr);

    printf("\nConstruction des tables de Huffman de la composante CbCr\n");
    huff_table *htable_CbCr_DC = huffman_table_build(htables_nb_symb_per_lengths[DC][Cb], htables_symbols[DC][Cb], htables_nb_symbols[DC][Cb]);
    if (htable_CbCr_DC == NULL) {
        jpeg_destroy(jpg);
        exit(EXIT_FAILURE);
    }
    jpeg_set_huffman_table(jpg, DC, Cb, htable_CbCr_DC);

    huff_table *htable_CbCr_AC = huffman_table_build(htables_nb_symb_per_lengths[AC][Cb], htables_symbols[AC][Cb], htables_nb_symbols[AC][Cb]);
    if (htable_CbCr_AC == NULL) {
        jpeg_destroy(jpg);
        exit(EXIT_FAILURE);
    }
    jpeg_set_huffman_table(jpg, AC, Cb, htable_CbCr_AC);
}

static void jpeg_set_sampling_factors(jpeg *jpg, const uint8_t sampling_factors[NB_COLOR_COMPONENTS][NB_DIRECTIONS]) {
    printf("\nEcriture des sampling-factors\n");
    for (enum color_component cc = Y; cc < NB_COLOR_COMPONENTS; cc++) {
        for (enum direction dir = H; dir < NB_DIRECTIONS; dir++) {
            jpeg_set_sampling_factor(jpg, cc, dir, sampling_factors[cc][dir]);
        }
    }
}

/*Programme principal*/
int main(int argc, char *argv[]) {
    /* Traitement des arguments */
    arguments args = {0};
    // Nécessaire, on test que les strings sont vides
    args.sample = "";
    args.outfile = "";

    /* Documentation */
    char doc[] =
        "Application qui convertit une image PPM / PGM en une image au format JPEG.\n\n \
                ./ppm2jpeg --outfile=sortie.jpeg --sample=hvx1,hvx2,hvx3 input.pgm";

    /* Structure des options */
    static struct argp_option options[] = {
        {"outfile", 'o', "sortie.jpg", 0, "Output to sortie.jpg instead of standard output.", 0},
        {"sample", 's', "hxv", 0, "Définie les facteurs d'échantillonnage hvx=h1xv1,h2xv2,h3xv3 des trois composantes de couleur.", 0},
        {0}};

    /* Structure argp */
    argp argparser = {options, parse_option, 0, doc, 0, 0, 0};

    /*Parsing des arguments*/
    error_t error = argp_parse(&argparser, argc, argv, 0, 0, &args);
    if (error != 0) exit(EXIT_FAILURE);

    /*
        Tout ce qui suit peut être mis dans une fonction de traitement des arguments.
    */

    verifier_syntaxe(args);

    /* Ecriture du nom de fichier de sortie */
    char *outfile;
    if (strlen(args.outfile) == 0) {
        outfile = malloc(strlen(args.inputfile) + 1 + 5);
        if (outfile == NULL) exit(EXIT_FAILURE);
        strcpy(outfile, args.inputfile);
        strip_ext(outfile);
        strcat(outfile, ".jpg");
    } else {
        outfile = malloc(strlen(args.outfile) + 1);
        strcpy(outfile, args.outfile);
    }

    /* Default sampling-factors */
    uint8_t sampling_factors[NB_COLOR_COMPONENTS][NB_DIRECTIONS] = {{1, 1}, {1, 1}, {1, 1}};
    bool no_downsampling = true;
    /* Stockage des sampling-factors */
    if (strlen(args.sample) != 0) {
        const char *separators = "x,";
        char *sample_copy = strdup(args.sample);
        char *strToken = strtok(sample_copy, separators);
        char *endPtr = NULL;
        for (size_t cc = Y; cc < NB_COLOR_COMPONENTS; cc++) {
            for (size_t dir = H; dir < NB_DIRECTIONS; dir++) {
                sampling_factors[cc][dir] = (uint8_t)strtol(strToken, &endPtr, 10);
                if (strToken == endPtr) exit(EXIT_FAILURE);
                if (sampling_factors[cc][dir] != 1) no_downsampling &= false;
                strToken = strtok(NULL, separators);
            }
        }
        free(sample_copy);
    }

    afficher_options(&args, outfile, sampling_factors, no_downsampling);
    if (!verifier_facteurs(sampling_factors)) {
        printf("les coefficients d'échantillonnage ne sont pas valides.\n");
        exit(EXIT_FAILURE);
    }

    /*
        Création du fichier de sortie et traitement des arguments en ligne de commande
    */

    jpeg *jpg = jpeg_create();

    jpeg_set_jpeg_filename(jpg, outfile);
    jpeg_set_ppm_filename(jpg, args.inputfile);

    jpeg_set_sampling_factors(jpg, sampling_factors);

    /*
        Compression du fichier d'entrée
    */

    /* Ouverture du fichier d'entrée */
    FILE *fichier = fopen(args.inputfile, "r");
    if (fichier == NULL) {
        perror("Error opening file ");
        exit(EXIT_FAILURE);
    }

    /*Parsing en-tête*/
    image_ppm *image = parse_entete(fichier);

    jpeg_set_nb_components(jpg, image->nb_components);
    uint8_t nb_components = jpeg_get_nb_components(jpg);

    jpeg_set_image_width(jpg, image->largeur);
    jpeg_set_image_height(jpg, image->hauteur);

    jpeg_set_tables_Y(jpg);
    if (nb_components == 3) jpeg_set_tables_CbCr(jpg);

    printf("Ecriture du header\n");
    jpeg_write_header(jpg);  // et création du bitstream

    MCUs *mcu = initialiser_MCUs(image, sampling_factors);
    afficher_image(image);
    /* Traitement des MCUs */

    bitstream *stream = jpeg_get_bitstream(jpg);
    huff_table *Y_dc_table = jpeg_get_huffman_table(jpg, DC, Y);
    if (Y_dc_table == NULL) exit(EXIT_FAILURE);
    huff_table *Y_ac_table = jpeg_get_huffman_table(jpg, AC, Y);
    if (Y_ac_table == NULL) exit(EXIT_FAILURE);
    huff_table *CbCr_dc_table = NULL;
    huff_table *CbCr_ac_table = NULL;
    if (nb_components == 3) {
        CbCr_dc_table = jpeg_get_huffman_table(jpg, DC, Cb);
        if (CbCr_dc_table == NULL) exit(EXIT_FAILURE);
        CbCr_ac_table = jpeg_get_huffman_table(jpg, AC, Cb);
        if (CbCr_ac_table == NULL) exit(EXIT_FAILURE);
    }

    int16_t difference_DC_Y = 0;
    int16_t difference_DC_Cb = 0;
    int16_t difference_DC_Cr = 0;

    /* Allocation d'une Data Unit 8x8 */
    int16_t **data_unit = malloc(8 * sizeof(*data_unit));
    if (data_unit == NULL) exit(EXIT_FAILURE);
    for (size_t i = 0; i < 8; i++) {
        data_unit[i] = malloc(8 * sizeof(data_unit[i]));
        if (data_unit[i] == NULL) exit(EXIT_FAILURE);
    }
    int16_t data_unit_freq[TAILLE_DATA_UNIT][TAILLE_DATA_UNIT] = {{0}};

    /*
        Il faut distinguer ici:
            si l'image est RGB ou Grayscale
            si l'image est RGB avec des facteurs d'échantillonnages quelconques
            ou RGB avec des facteurs d'échantillonnages donnant des mcus de taille 8x8
    */
    for (size_t m = 0; m < image->nb_MCUs; m++) {
        printf("\nTraitement du mcu %zu\n", m);
        recuperer_MCUs(fichier, image, mcu);
        afficher_MCUs(nb_components, mcu);

        if (no_downsampling) {
            printf("No Downsampling");
            /* Image Grayscale (sans facteurs ie. 1x1 1x1 1x1) */
            // on encode directement mcu->Y
            encode_data_unit(mcu->Y, data_unit_freq, quantification_table_Y);
            ecrire_coeffs(stream, data_unit_freq, Y_dc_table, Y_ac_table, difference_DC_Y);
            difference_DC_Y = data_unit_freq[0][0];

            /* Image RGB sans facteurs */
            // on encode directement mcu->Cb et mcu->Cr
            if (nb_components == 3) {
                // encode_data_unit(mcu->Y, data_unit_freq);
                // ecrire_coeffs(stream, data_unit_freq, Y_dc_table, Y_ac_table, difference_DC);
                encode_data_unit(mcu->Cb, data_unit_freq, quantification_table_CbCr);
                ecrire_coeffs(stream, data_unit_freq, CbCr_dc_table, CbCr_ac_table, difference_DC_Cb);
                difference_DC_Cb = data_unit_freq[0][0];

                encode_data_unit(mcu->Cr, data_unit_freq, quantification_table_CbCr);
                ecrire_coeffs(stream, data_unit_freq, CbCr_dc_table, CbCr_ac_table, difference_DC_Cr);
                difference_DC_Cr = data_unit_freq[0][0];
            }
        } else {
            /* Image RGB avec facteurs */
            /* Traitement des blocs Y */
            difference_DC_Y = process_Y(mcu->Y, sampling_factors[Y][H], sampling_factors[Y][V],
                                        data_unit, data_unit_freq,
                                        Y_dc_table, Y_ac_table,
                                        stream, difference_DC_Y);
            /* Mise à zéro de data_unit */
            for (size_t i = 0; i < 8; i++) {
                for (size_t j = 0; j < 8; j++) {
                    data_unit[i][j] = 0;
                }
            }

            difference_DC_Cb = process_chroma(mcu->Cb, sampling_factors[Y][H], sampling_factors[Y][V],
                                              sampling_factors[Cb][H], sampling_factors[Cb][V],
                                              data_unit, data_unit_freq,
                                              CbCr_dc_table, CbCr_ac_table,
                                              stream, difference_DC_Cb);

            difference_DC_Cr = process_chroma(mcu->Cr, sampling_factors[Y][H], sampling_factors[Y][V],
                                              sampling_factors[Cr][H], sampling_factors[Cr][V],
                                              data_unit, data_unit_freq,
                                              CbCr_dc_table, CbCr_ac_table,
                                              stream, difference_DC_Cr);
        }

        printf("\nEnd of %zu MCU\n", m);
    }

    /*Fermeture du fichier d'entrée */
    fclose(fichier);

    free_mcu(&mcu);
    free(image);

    // printf("Ecriture du footer\n");
    jpeg_write_footer(jpg);

    printf("Destruction de la structure jpeg\n");
    jpeg_destroy(jpg);

    /* Libération d'un bloc 8x8 */
    for (size_t i = 0; i < 8; i++) {
        free(data_unit[i]);
    }
    free(data_unit);  // double free or corruption (out)

    free(outfile);

    return EXIT_SUCCESS;
}
