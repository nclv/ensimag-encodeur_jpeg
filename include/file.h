#ifndef _FILE_H_
#define _FILE_H_

#include "huffman.h"

typedef struct element {
    Node *node;
    struct element *next;
} element;

typedef struct file {
    element *first;
} file;

extern file *creer_file(void);

extern void liberer_file(file **f);

extern void enfiler(file *f, Node *node);

extern Node *defiler(file *f);

extern void afficher_file(file *f);

#endif /* _FILE_H_ */