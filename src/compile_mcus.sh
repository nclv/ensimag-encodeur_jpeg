#!/bin/bash

gcc -c -std=c99 ppm.c ../include/ppm.h
gcc -c -std=c99 mcus.c ../include/mcus.h
gcc -c -std=c99 test_mcus.c
gcc *.o -o test_mcus
