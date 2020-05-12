#!/bin/bash

gcc -c -std=c99 ppm.c ../include/ppm.h
gcc -c -std=c99 test_ppm.c
gcc *.o -o test_ppm
