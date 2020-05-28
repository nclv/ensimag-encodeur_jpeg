#ifndef _PPM2JPEG_H_
#define _PPM2JPEG_H_

/* Structure des arguments */
typedef struct arguments {
    const char *inputfile;
    const char *outfile;
    const char *sample;
} arguments;

typedef struct argp argp;

enum processing {
    GRAYSCALE,
    RGB_WITH_SAMPLING,
    RGB_WITHOUT_SAMPLING,
    NB_PROCESSING
};

#endif