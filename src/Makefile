# UTILISATION
# make
# make clean
# make test_conversion équivalent à gcc conversion.c test_conversion.c -I../include -o test_conversion
# make test_mcus
# ...
# make clean_executable

CC = gcc
LD = gcc
INCLUDES = -I../include

# -O0 désactive les optimisations à la compilation
# C'est utile pour débugger, par contre en "production"
# on active au moins les optimisations de niveau 2 (-O2).
CFLAGS = -std=c99 -Wall -Wextra -g3 -O0 -Werror -Wshadow -Wconversion -Wdouble-promotion -Wformat=2 -Wformat-truncation=2 -Wundef -fno-common -fstack-usage -Wfloat-equal -Wpointer-arith -Wcast-align -Wstrict-prototypes -Wstrict-overflow=5 -Wwrite-strings -Waggregate-return -ffunction-sections -fdata-sections
LDFLAGS = -Wl,--gc-sections -Wl,--print-gc-sections

# typing 'make' will invoke the first target entry in the file 

TARGET = test_conversion test_mcus test_ppm
OBJS = conversion.o test_conversion.o mcus.o test_mcus.o

all: $(TARGET)

test_conversion: conversion.o test_conversion.o
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^ $(LDFLAGS)

test_mcus: mcus.o test_mcus.o
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^ $(LDFLAGS)

test_ppm: ppm.o test_ppm.o
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^ $(LDFLAGS)

# this is a suffix replacement rule for building .o's from .c's
# it uses automatic variables $<: the name of the prerequisite of
# the rule(a .c file) and $@: the name of the target of the rule (a .o file)
# .c.o:
# 	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
$(OBJS): %.o: %.c
	$(CC) -c $(CFLAGS) $(INCLUDES) $< -o $@ 

.PHONY: clean clean_executable

clean:
	rm -rf *.su *.o *~

clean_executable:
	rm -rf $(TARGET)
