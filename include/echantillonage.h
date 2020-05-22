#ifndef _ECHANTILLONAGE_H_
#define _ECHANTILLONAGE_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef enum ech {
	SIMPLE,
	HORIZONTAL,
	VERTICAL_HORIZONTAL
} ech;

extern ech recuperer_echantillonage(uint8_t h, uint8_t v);

#endif
