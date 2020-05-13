gcc -c -std=c99 src/ppm.c include/ppm.h
gcc -c -std=c99 src/conversion.c include/conversion.h
gcc -c -std=c99 src/test_ppm.c
gcc *.o -o test_ppm

