# UTILISATION
# make
# make clean
# make test_conversion équivalent à gcc conversion.c test_conversion.c -I../include -o test_conversion
# make test_mcus
# ...
# make clean_executable

CC = gcc
LD = gcc
INCLUDE = ../include
INCLUDES = -I$(INCLUDE)

# -O0 désactive les optimisations à la compilation
# C'est utile pour débugger, par contre en "production"
# on active au moins les optimisations de niveau 2 (-O2).
CFLAGS = -std=c99 -Wall -Wextra -g3 -O0 -Werror -Wshadow -Wconversion -Wdouble-promotion -Wformat=2 -Wformat-truncation=2 -Wundef -fno-common -fstack-usage -Wfloat-equal -Wpointer-arith -Wcast-align -Wstrict-prototypes -Wstrict-overflow=5 -Wwrite-strings -Waggregate-return -ffunction-sections -fdata-sections
LDFLAGS = -Wl,--gc-sections -Wl,--print-gc-sections

# typing 'make' will invoke the first target entry in the file 

TARGET = test_conversion test_mcus test_ppm test_bitstream
OBJS = conversion.o test_conversion.o mcus.o test_mcus.o ppm.o test_ppm.o bitstream.o test_bitstream.o

all: $(TARGET)

test_conversion: conversion.o test_conversion.o
	$(info )
	$(info Compilation de test_conversion)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^ $(LDFLAGS)

test_mcus: mcus.o test_mcus.o
	$(info )
	$(info Compilation de test_mcus)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^ $(LDFLAGS)

test_ppm: ppm.o test_ppm.o conversion.o
	$(info )
	$(info Compilation de test_ppm)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^ $(LDFLAGS)

test_bitstream: bitstream.o test_bitstream.o
	$(info )
	$(info Compilation de test_bitstream)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^ $(LDFLAGS)

# this is a suffix replacement rule for building .o's from .c's
# it uses automatic variables $<: the name of the prerequisite of
# the rule(a .c file) and $@: the name of the target of the rule (a .o file)
# .c.o:
# 	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
$(OBJS): %.o: %.c
	$(info )
	$(info Compilation des fichiers .o)
	$(CC) -c $(CFLAGS) $(INCLUDES) $< -o $@ 

.PHONY: clean clean_executable

clean:
	$(info Suppression des .su, .o, .h.gch)
	rm -rf *.su *.o *~ $(INCLUDE)/*.h.gch

clean_executable:
	$(info Suppression des exécutables)
	rm -rf $(TARGET)
