#include <argp.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdint.h>

#include "ppm.h"
#include "ppm2jpeg.h"
#include "utils.h"

/*Documentation*/
char doc[] = "Application qui convertit une image PPM en une image au format JPEG.\n\n \
            ./ppm2jpeg --outfile=sortie.jpeg --sample=hvx1,hvx2,hvx3 input.pgm";

/*Structure des options*/
static struct argp_option options[] = {
    {"outfile", 'o', "sortie.jpg", 0, "Output to sortie.jpg instead of standard output.", 0},
    {"sample", 's', "hxv", 0, "Définie les facteurs d'échantillonnage hvx=h1xv1,h2xv2,h3xv3 des trois composantes de couleur.", 0},
    {0}};

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
bool nb_points(const char* name) {
	uint32_t cmp = 0;
	size_t i = 0;

	while (cmp < 2 && name[i] != '\0') {
		if (name[i] == '.') cmp += 1;
		i++;
	}

	return (cmp == 1);
}

bool extension(const char* name, const char* extension) {
  return !strcmp(name, extension);
}

/*Renvoie True si le caractère c est un digit*/
bool is_digit(const char c) {
	return c >= '0' && c <= '9';
}

/*Vérifie la syntaxe des arguments donnés*/
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

/*Vérifie les spécifications de la norme JPEG*/
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

void verifier_syntaxe(arguments args) {
  if (!(nb_points(args.inputfile) && nb_points(args.outfile))) {
    printf("ERREUR: Trop ou pas assez de points dans les noms de fichiers IO\n");
    exit(EXIT_FAILURE);
  }
  if (!(extension(strchr(args.inputfile, '.'), ".ppm") || extension(strchr(args.inputfile, '.'), ".pgm"))) {
    printf("ERREUR: Les fichiers d'entrée ne sont pas reconnus\n");
    exit(EXIT_FAILURE);
  }
  if (!extension(strchr(args.outfile, '.'), ".jpg")) {
    printf("ERREUR: Le fichier de sortie n'est pas reconnu\n");
    exit(EXIT_FAILURE);
  }
  if (extension(strchr(args.inputfile, '.'), ".ppm") && !strcmp(args.sample, "")) {
    printf("ERREUR: Les facteurs d'échantillonage ne sont pas spécifiés.\n");
    exit(EXIT_FAILURE);
  }
  else if (!verifier_sample(args.sample)) {
    printf("ERREUR: Le format des facteurs d'échantillonage est incorrect\n");
    exit(EXIT_FAILURE);
  }
}

/*Structure argp*/
struct argp argp = {options, parse_option, 0, doc, 0, 0, 0};

/*Programme principal*/
int main(int argc, char *argv[]) {
	/*Traitement des arguments*/
	arguments args = {0};
  args.sample = "";
  args.outfile = "output.jpg";

  /*Parsing des arguments*/
  error_t error = argp_parse(&argp, argc, argv, 0, 0, &args);
  if (error != 0) exit(EXIT_FAILURE);

  verifier_syntaxe(args);

  /*Stockage sample*/
  const char *separators = "x,";
  char *sample_copy = strdup(args.sample);
  char *strToken = strtok(sample_copy, separators);
  uint8_t facteurs[NOMBRE_FACTEURS];
  for (size_t i = 0; i < NOMBRE_FACTEURS; i++) {
      facteurs[i] = (uint8_t)atoi(strToken);
      strToken = strtok(NULL, separators);
  }
  printf("%p\n", facteurs);

  free(sample_copy);

  printf("input file: %s\noutput file: %s\nsample: %s\n", args.inputfile, args.outfile, args.sample);




    return EXIT_SUCCESS;
}
