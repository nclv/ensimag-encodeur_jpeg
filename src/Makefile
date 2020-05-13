CC = gcc
LD = gcc

# -O0 désactive les optimisations à la compilation
# C'est utile pour débugger, par contre en "production"
# on active au moins les optimisations de niveau 2 (-O2).
CFLAGS = -std=c99 -Wall -Wextra -g3 -O0 -Werror -Wshadow -Wconversion -Wdouble-promotion -Wformat=2 -Wformat-truncation=2 -Wundef -fno-common -fstack-usage -Wfloat-equal -Wpointer-arith -Wcast-align -Wstrict-prototypes -Wstrict-overflow=5 -Wwrite-strings -Waggregate-return -ffunction-sections -fdata-sections -I../include
LDFLAGS = -Wl,--gc-sections -Wl,--print-gc-sections

TESTS = test_conversion \
	test_mcu \
	test_ppm
SRC = $(TESTS:=.c)
OBJ = $(TESTS:=.o)

all: $(TESTS)

ppm2jpeg: $(OBJ_FILES) $(OBJ_PROF_FILES)
	$(LD) $(LDFLAGS) $(OBJ_FILES) $(OBJ_PROF_FILES) -o $@

%-test: %.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $^

.PHONY: clean

clean:
	rm -rf *.o *~ $(TESTS)
