#include <argp.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

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

int main(int argc, char *argv[]) {
    arguments args = {0};
    // specify argument defaults
    args.sample = "";
    args.outfile = "-";  // default to stdout

    error_t error = argp_parse(&argp, argc, argv, 0, 0, &args);
    if (error != 0) exit(EXIT_FAILURE);
    printf("input file: %s\noutput file: %s\nsample: %s\n", args.inputfile, args.outfile, args.sample);

    const char *separators = "x,";
    char *sample_copy = strdup(args.sample);
    char *strToken = strtok(sample_copy, separators);
    while (strToken != NULL) {
        printf("%s\n", strToken);
        strToken = strtok(NULL, separators);
    }
    free(sample_copy);

    return EXIT_SUCCESS;
}
