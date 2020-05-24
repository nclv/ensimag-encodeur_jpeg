#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "compression.h"
#include "structures.h"

int main(int argc, char** argv) {
	if (argc != 8) {
		fprintf(stderr, "ERREUR: /.%s <Nom_Fichier> <h1> <h2> <h3> <v1> <v2> <v3>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	facteurs[H1] = (uint8_t)atoi(argv[2]);
	facteurs[H2] = (uint8_t)atoi(argv[3]);
	facteurs[H3] = (uint8_t)atoi(argv[4]);
	facteurs[V1] = (uint8_t)atoi(argv[5]);
	facteurs[V2] = (uint8_t)atoi(argv[6]);
	facteurs[V3] = (uint8_t)atoi(argv[7]);

	bool retour = compression(argv[1]);

	if (retour == true) {
		printf("PASSED\n");
	}

	return EXIT_SUCCESS;
}
