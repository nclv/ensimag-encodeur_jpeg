#include "utils.h"

#include <stdlib.h>
#include <string.h>

char *strdup(const char *str) {
    size_t len = strlen(str) + 1;
    void *new = malloc(len);
    if (new == NULL)
        return NULL;
    return (char *)memcpy(new, str, len);
}

void strip_ext(char *filename) {
    char *end = filename + strlen(filename);

    while (end > filename && *end != '.' && *end != '\\' && *end != '/') {
        --end;
    }
    if ((end > filename && *end == '.') &&
        (*(end - 1) != '\\' && *(end - 1) != '/')) {
        *end = '\0';
    }
}