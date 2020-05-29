CC = gcc -pg
LD = gcc -pg

# -O0 désactive les optimisations à la compilation
# C'est utile pour débugger, par contre en "production"
# on active au moins les optimisations de niveau 2 (-O2).
CFLAGS = -std=c99 -Wall -Wextra -g3 -O3 -Werror -Wshadow -Wconversion -Wdouble-promotion -Wformat=2 -Wformat-truncation=2 -Wundef -fno-common -fstack-usage -fstack-protector-all -Wfloat-equal -Wpointer-arith -Wcast-align -Wstrict-prototypes -Wstrict-overflow=5 -Wwrite-strings -Waggregate-return -ffunction-sections -fdata-sections -Iinclude/
LDFLAGS = -Wl,--gc-sections -Wl,--print-gc-sections -Wl,-z,relro,-z,now

# Par défaut, on compile tous les fichiers source (.c) qui se trouvent dans le
# répertoire src/
SRC_FILES=$(wildcard src/*.c)

# Par défaut, la compilation de src/toto.c génère le fichier objet obj/toto.o
OBJ_FILES=$(patsubst src/%.c,obj/%.o,$(SRC_FILES))

# Fichiers objet "prof". Lorsque vous aurez implémenté un de ces modules, il
# faudra le retirer de cette liste pour lier ppm2jpeg à votre implémentation du
# module en question. Le module htables_prof.o, qui contient la déclaration des tables
# de Huffman génériques sous forme de constantes, n'est pas à ré-implémenter.
OBJ_PROF_FILES = obj_prof/htables_prof.o

all: bin/ppm2jpeg

bin/ppm2jpeg: $(OBJ_FILES) $(OBJ_PROF_FILES)
	$(LD) $(LDFLAGS) $(OBJ_FILES) $(OBJ_PROF_FILES) -o $@

obj/%.o: src/%.c
	$(CC) -c $(CFLAGS) $< -o $@

.PHONY: clean

clean:
	rm -rf ppm2jpeg $(OBJ_FILES) obj/*.su
