#include "ppm2jpeg.h"

#include <argp.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dct.h"
#include "downsampling.h"
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

/*Vérifie que le nom du fichier IO est correct
 *C'est à dire un seul point et une extension donné
 *Renvoie true si le nombre de points est == 1*/
bool check_one_dot(const char *name) {
    uint32_t cmp = 0;
    size_t i = 0;

    while (cmp < 2 && name[i] != '\0') {
        if (name[i] == '.') cmp += 1;
        i++;
    }

    return (cmp == 1);
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

void afficher_traitement_dynamique(int16_t **input, const char *chaine) {
    printf("%s\n", chaine);
    for (size_t i = 0; i < TAILLE_DATA_UNIT; i++) {
        for (size_t j = 0; j < TAILLE_DATA_UNIT; j++) {
            printf("%02hhX ", input[i][j]);
        }
        printf("\n");
    }
}

void afficher_traitement_statique(int16_t input[8][8], const char *chaine) {  //Merci les [8][8] <3
    printf("%s\n", chaine);
    for (size_t i = 0; i < TAILLE_DATA_UNIT; i++) {
        for (size_t j = 0; j < TAILLE_DATA_UNIT; j++) {
            printf("%d ", input[i][j]);
        }
        printf("\n");
    }
}

void encode(int16_t **input, bool choix_YCbCr) {
    int16_t output[TAILLE_DATA_UNIT][TAILLE_DATA_UNIT];
    afficher_traitement_dynamique(input, "0)Affichage avant traitement");
    offset(input);
    afficher_traitement_dynamique(input, "1)Affichage offset");
    dct(input, output);
    afficher_traitement_statique(output, "2)Affichage dct");
    zigzag_inplace(output);
    afficher_traitement_statique(output, "3)Affichage zig-zag");
    quantifier(output, (choix_YCbCr) ? quantification_table_Y : quantification_table_CbCr);
    afficher_traitement_statique(output, "4)Affichage quantification");
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
        "Application qui convertit une image PPM en une image au format JPEG.\n\n \
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
                strToken = strtok(NULL, separators);
            }
        }
        free(sample_copy);
    }

    printf("input file: %s\noutput file: %s\n", args.inputfile, outfile);
    for (size_t cc = Y; cc < NB_COLOR_COMPONENTS; cc++) {
        for (size_t dir = H; dir < NB_DIRECTIONS; dir++) {
            printf("%d ", sampling_factors[cc][dir]);
        }
    }
    printf("\n");

    /*
        Création du fichier de sortie
    */

    jpeg *jpg = jpeg_create();

    jpeg_set_jpeg_filename(jpg, outfile);
    jpeg_set_ppm_filename(jpg, args.inputfile);

    printf("Ecriture des sampling-factors\n");
    for (size_t cc = Y; cc < NB_COLOR_COMPONENTS; cc++) {
        for (size_t dir = H; dir < NB_DIRECTIONS; dir++) {
            jpeg_set_sampling_factor(jpg, cc, dir, sampling_factors[cc][dir]);
        }
    }

    jpeg_set_quantization_table(jpg, Y, quantification_table_Y);
    // jpeg_set_quantization_table(jpg, Cb, quantification_table_CbCr);

    printf("Construction des tables de Huffman\n");
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

    // huff_table *htable_CbCr_DC = huffman_table_build(htables_nb_symb_per_lengths[DC][Cb], htables_symbols[DC][Cb], htables_nb_symbols[DC][Cb]);
    // if (htable_CbCr_DC == NULL) {
    //     jpeg_destroy(jpg);
    //     exit(EXIT_FAILURE);
    // }
    // jpeg_set_huffman_table(jpg, DC, Cb, htable_CbCr_DC);

    // huff_table *htable_CbCr_AC = huffman_table_build(htables_nb_symb_per_lengths[AC][Cb], htables_symbols[AC][Cb], htables_nb_symbols[AC][Cb]);
    // if (htable_CbCr_AC == NULL) {
    //     jpeg_destroy(jpg);
    //     exit(EXIT_FAILURE);
    // }
    // jpeg_set_huffman_table(jpg, AC, Cb, htable_CbCr_AC);

    /*
        Compression du fichier d'entrée
    */

    /* Ouverture du fichier d'entrée */
    FILE *fichier = fopen(args.inputfile, "r");
    if (fichier == NULL) exit(EXIT_FAILURE);

    /*Parsing en-tête*/
    image_ppm *image = parse_entete(fichier);

    jpeg_set_nb_components(jpg, 1);
    // jpeg_set_nb_components(jpg, 3);

    jpeg_set_image_width(jpg, image->largeur);
    jpeg_set_image_height(jpg, image->hauteur);

    printf("Ecriture du header\n");
    jpeg_write_header(jpg);  // et création du bitstream

    MCUs *mcu = initialiser_MCUs(image, sampling_factors);

    /* Allocation d'une Data Unit 8x8 */
    int16_t **data_unit = calloc(8, sizeof *data_unit);
    if (data_unit == NULL) exit(EXIT_FAILURE);
    for (size_t i = 0; i < 8; i++) {
        data_unit[i] = calloc(8, sizeof *data_unit[i]);
        if (data_unit[i] == NULL) exit(EXIT_FAILURE);
    }

    /* Traitement des MCUs */

    bitstream *stream = jpeg_get_bitstream(jpg);
    huff_table *Y_dc_table = jpeg_get_huffman_table(jpg, DC, Y);
    huff_table *Y_ac_table = jpeg_get_huffman_table(jpg, AC, Y);

    int16_t difference_DC = 0;

    /*
        Il faut distinguer ici:
            si l'image est RGB ou Grayscale
            si l'image est RGB avec des facteurs d'échantillonnages quelconques
            ou RGB avec des facteurs d'échantillonnages donnant des mcus de taille 8x8
    */

    int16_t data_unit_freq[TAILLE_DATA_UNIT][TAILLE_DATA_UNIT] = {0};
    for (size_t i = 0; i < image->nb_MCUs; i++) {
        printf("Traitement du mcu %ld\n", i + 1);
        recuperer_MCUs(fichier, image, mcu);
        afficher_MCUs(image->format, mcu);

        /* Image RGB avec facteurs */
        // process_Y(mcu->Y, sampling_factors[Y][H], sampling_factors[Y][V], data_unit);

        // process_chroma(mcu->Cb, sampling_factors[Y][H], sampling_factors[Y][V], sampling_factors[Cb][H], sampling_factors[Cb][V], data_unit);
        // process_chroma(mcu->Cr, sampling_factors[Y][H], sampling_factors[Y][V], sampling_factors[Cr][H], sampling_factors[Cr][V], data_unit);

        /* Image Grayscale */
        // on encode directement mcu->Y
        offset(mcu->Y);
        afficher_traitement_dynamique(mcu->Y, "Offset ");
        dct(mcu->Y, data_unit_freq);
        printf("DCT: \n");
        for (size_t k = 0; k < TAILLE_DATA_UNIT; k++) {
            for (size_t j = 0; j < TAILLE_DATA_UNIT; j++) {
                printf("%04x ", data_unit_freq[k][j]);
            }
            printf("\n");
        }
        zigzag_inplace(data_unit_freq);
        printf("Zig-Zag: \n");
        for (size_t k = 0; k < TAILLE_DATA_UNIT; k++) {
            for (size_t j = 0; j < TAILLE_DATA_UNIT; j++) {
                printf("%04x ", data_unit_freq[k][j]);
            }
            printf("\n");
        }
        quantifier(data_unit_freq, quantification_table_Y);
        printf("Quantification: \n");
        for (size_t k = 0; k < TAILLE_DATA_UNIT; k++) {
            for (size_t j = 0; j < TAILLE_DATA_UNIT; j++) {
                printf("%04x ", data_unit_freq[k][j]);
            }
            printf("\n");
        }
        ecrire_coeffs(stream, data_unit_freq, Y_dc_table, Y_ac_table, difference_DC);

        printf("End of %ld Data Unit", i);
        difference_DC = data_unit_freq[0][0];

        /* Image RGB sans facteurs (1x1 1x1 1x1) */
        // on encode directement mcu->Y, mcu->Cb et mcu->Cr
    }

    /* Après écriture du footer, hexdump -C ne renvoie plus les paramètres du header */
    // printf("Ecriture du footer\n");
    // jpeg_write_footer(jpg);

    printf("Destruction de la structure jpeg\n");
    jpeg_destroy(jpg);

    /* Libération d'un bloc 8x8 */
    for (size_t i = 0; i < 8; i++) {
        free(data_unit[i]);
    }
    free(data_unit);

    free(outfile);

    /*Fermeture du fichier d'entrée */
    fclose(fichier);

    return EXIT_SUCCESS;
}
