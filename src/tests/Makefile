# UTILISATION
# make
# make test_conversion équivalent à gcc conversion.c test_conversion.c -I../include -o test_conversion
# make test_mcus
# ...
# make clean
# make clean_executable
# make clean_all

CC = gcc
LD = gcc
INCLUDE = ../../include
INCLUDES = -I$(INCLUDE)
SRC = ../

# -O0 désactive les optimisations à la compilation
# C'est utile pour débugger, par contre en "production"
# on active au moins les optimisations de niveau 2 (-O2).
CFLAGS = -std=c99 -Wall -Wextra -g3 -O0 -Werror -Wshadow -Wconversion -Wdouble-promotion -Wformat=2 -Wformat-truncation=2 -Wundef -fno-common -fstack-usage -Wfloat-equal -Wpointer-arith -Wcast-align -Wstrict-prototypes -Wstrict-overflow=5 -Wwrite-strings -Waggregate-return -ffunction-sections -fdata-sections
LDFLAGS = -Wl,--gc-sections -Wl,--print-gc-sections

# typing 'make' will invoke the first target entry in the file 

TARGET = test_conversion test_ppm test_bitstream test_dct test_zigzag test_quantification\
	test_jpeg_writer test_file
OBJS = $(SRC)conversion.o test_conversion.o \
	$(SRC)ppm.o test_ppm.o \
	$(SRC)bitstream.o test_bitstream.o\
	$(SRC)dct.o test_dct.o\
	$(SRC)zigzag.o test_zigzag.o\
	$(SRC)quantification.o test_quantification.o\
	$(SRC)echantillonage.o\
	$(SRC)jpeg_writer.o test_jpeg_writer.o\
	$(SRC)file.o test_file.o\
	$(SRC)huffman.o test_huffman.o

all: $(TARGET)

test_conversion: $(SRC)conversion.o test_conversion.o
	$(info )
	$(info Compilation de test_conversion)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^ $(LDFLAGS)

test_ppm: $(SRC)ppm.o test_ppm.o $(SRC)conversion.o $(SRC)echantillonage.o
	$(info )
	$(info Compilation de test_ppm)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^ $(LDFLAGS)

test_bitstream: $(SRC)bitstream.o test_bitstream.o
	$(info )
	$(info Compilation de test_bitstream)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^ $(LDFLAGS)

test_dct: $(SRC)dct.o test_dct.o
	$(info )
	$(info Compilation de test_dct)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^ $(LDFLAGS)

test_zigzag: $(SRC)zigzag.o test_zigzag.o
	$(info )
	$(info Compilation de test_zigzag)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^ $(LDFLAGS)

test_quantification: $(SRC)quantification.o test_quantification.o
	$(info )
	$(info Compilation de test_quantification)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^ $(LDFLAGS)

test_jpeg_writer: $(SRC)jpeg_writer.o test_jpeg_writer.o
	$(info )
	$(info Compilation de test_jpeg_writer)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^ $(LDFLAGS)

test_file: $(SRC)file.o $(SRC)huffman.o test_file.o
	$(info )
	$(info Compilation de test_file)
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

.PHONY: clean clean_executable clean_all

clean_all: clean clean_executable

clean:
	$(info Suppression des .su, .o, .h.gch)
	rm -rf *.su *.o $(SRC)*.su $(SRC)*.o *~ $(INCLUDE)/*.h.gch

clean_executable:
	$(info Supression des exécutables)
	rm -rf $(TARGET)
