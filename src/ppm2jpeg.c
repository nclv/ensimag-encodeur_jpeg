#include <argp.h>
#include <stdbool.h>

char doc[] = "Application qui convertit une image PPM en une image au format JPEG.\n\n./ppm2jpeg --outfile=sortie.jpeg --sample=hvx1,hvx2,hvx3 input.pgm";

static struct argp_option options[] = {
    {"outfile", 'o', "sortie.jpg", 0, "Output to sortie.jpg instead of standard output.", 0},
    {"sample", 's', "hvx", 0, "Définie les facteurs d'échantillonnage hvx=h1xv1,h2xv2,h3xv3 des trois composantes de couleur.", 0},
    {0}};

struct arguments {
    const char *inputfile;
    const char *outfile;
    const char *sample;
};

static error_t parse_option(int key, char *arg, struct argp_state *state) {
    // state->input points to our user-defined argument structure
    struct arguments *arguments = state->input;

    switch (key) {
        case ARGP_KEY_ARG:
            if (state->arg_num == 0) {
                arguments->inputfile = arg;
            } else {
                // the user provided too many arguments
                argp_usage(state);
            }
            break;
        case 'o':
            arguments->outfile = arg;
            break;
        case 's':
            arguments->sample = arg;
            break;
        case ARGP_KEY_END:
            // we are done processing arguments.
            // validate that user provided an input file
            if (arguments->inputfile == NULL) {
                argp_usage(state);
            }
            break;
        default:
            return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

struct argp argp = {options, parse_option, 0, doc, 0, 0, 0};

int main(int argc, char *argv[]) {
    struct arguments arguments = {0};
    // specify argument defaults
    arguments.sample = "";
    arguments.outfile = "-";  // default to stdout

    argp_parse(&argp, argc, argv, 0, 0, &arguments);
    printf("input file: %s\noutput file: %s\nsample: %s\n", arguments.inputfile, arguments.outfile, arguments.sample);
    return 0;
}