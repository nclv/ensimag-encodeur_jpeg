#ifndef _PPM2JPEG_H_
#define _PPM2JPEG_H_

#include <stdbool.h>

/* Structure des arguments */
typedef struct arguments {
    const char *inputfile;
    const char *outfile;
    const char *sample;
    bool verbose;
} arguments;

typedef struct argp argp;

#endif