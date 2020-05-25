#include <argp.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdint.h>

#include "ppm.h"

char doc[] = "Application qui convertit une image PPM en une image au format JPEG.\n\n./ppm2jpeg --outfile=sortie.jpeg --sample=hvx1,hvx2,hvx3 input.pgm";

static struct argp_option options[] = {
    {"outfile", 'o', "sortie.jpg", 0, "Output to sortie.jpg instead of standard output.", 0},
    {"sample", 's', "hxv", 0, "Définie les facteurs d'échantillonnage hvx=h1xv1,h2xv2,h3xv3 des trois composantes de couleur.", 0},
    {0}};

typedef struct arguments {
    const char *inputfile;
    const char *outfile;
    const char *sample;
} arguments;

static error_t parse_option(int key, char *arg, struct argp_state *state) {
    // state->input points to our user-defined argument structure
    arguments *args = state->input;

    switch (key) {
        case ARGP_KEY_ARG:
            if (state->arg_num == 0) {
                args->inputfile = arg;
            } else {
                // the user provided too many arguments
                argp_usage(state);
            }
            break;
        case 'o':
            args->outfile = arg;
            break;
        case 's':
            args->sample = arg;
            break;
        case ARGP_KEY_END:
            // we are done processing arguments.
            // validate that user provided an input file
            if (args->inputfile == NULL) {
                argp_usage(state);
            }
            break;
        default:
            return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

struct argp argp = {options, parse_option, 0, doc, 0, 0, 0};

char *strdup(const char *str) {
    size_t len = strlen(str) + 1;
    void *new = malloc(len);
    if (new == NULL)
        return NULL;
    return (char *)memcpy(new, str, len);
}

bool name_outfile_correct(const char* name) {
	uint32_t cmp = 0;
	size_t i = 0;
	size_t pos;

	while (cmp < 2 && name[i] != '\0') {
		if (name[i] == '.') {
			cmp += 1;
			pos = i;
		}
		i++;
	}

	return (cmp == 1 && !strcmp(&name[pos], ".jpg"));
}

bool is_digit(const char c) {
	return c >= '0' && c <= '9';
}

bool verifier_sample(const char* sample) {
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

bool verifier_facteurs(uint8_t facteurs[]) {
	bool est_correct = true;
	size_t i = 0;
	while (est_correct && i < NOMBRE_FACTEURS) {
		est_correct = est_correct && facteurs[i] >= 1 && facteurs[i] <= 4;
		i += 1;
	}
	est_correct = est_correct && (facteurs[H1] * facteurs[V1] + facteurs[H2] * facteurs[V2] + facteurs[H3] * facteurs[V3] <= 10);
	est_correct = est_correct && (facteurs[H1] % facteurs[H2] == 0 && facteurs[H1] % facteurs[H3] == 0 && facteurs[V1] % facteurs[V2] == 0 && facteurs[V1] % facteurs[V3] == 0);

	return est_correct;
}

int main(int argc, char *argv[]) {
	/*Traitement des arguments*/
	arguments args = {0};
    args.sample = "";
    args.outfile = "output.jpg";

    error_t error = argp_parse(&argp, argc, argv, 0, 0, &args);
    if (error != 0) exit(EXIT_FAILURE);
    printf("input file: %s\noutput file: %s\nsample: %s\n", args.inputfile, args.outfile, args.sample);

	/*Vérification args.outfile*/
	if (!name_outfile_correct(args.outfile)) {
		printf("Nom de fichier de sortie incorrect\n");
		exit(EXIT_FAILURE);
	}

    /*Vérification de sample si RGB*/
	char* index_point = index(args.inputfile, '.');
	if (index_point == NULL) {
		printf("Format d'inputfile incorrect\n");
		exit(EXIT_FAILURE);
	}
	if (!strcmp(index_point, ".ppm") && !strcmp(index_point, ".pgm")) {
		printf("Format d'image inconnu\n");
		exit(EXIT_FAILURE);
	}

	if (!strcmp(index(index_point, '.'), ".ppm") && !strcmp(args.sample, "")) {
		printf("Image au format RGB sans facteurs d'échantillonage... Sortie...\n");
		exit(EXIT_FAILURE);
    }
	else if (!verifier_sample(args.sample)) {
		printf("Format des facteurs d'échantillonage incorrects\n");
		exit(EXIT_FAILURE);
	}

    /*Stockage sample*/
    const char *separators = "x,";
    char *sample_copy = strdup(args.sample);
    char *strToken = strtok(sample_copy, separators);
    uint8_t facteurs[NOMBRE_FACTEURS];
    for (size_t i = 0; i < NOMBRE_FACTEURS; i++) {
        facteurs[i] = (uint8_t)atoi(strToken);
        strToken = strtok(NULL, separators);
    }

	if (!verifier_facteurs(facteurs)) {
		printf("Valeurs des facteurs d'échantillonage incorrects\n");
		exit(EXIT_FAILURE);
	}

    free(sample_copy);

	for (size_t i = 0; i < NOMBRE_FACTEURS; i++) {
		printf("facteurs = %d\n", facteurs[i]);
	}



    return EXIT_SUCCESS;
}
