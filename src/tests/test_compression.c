#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "compression.h"

int main(int argc, char** argv) {
	if (argc != 2) {
		fprintf(stderr, "ERREUR: /.%s <Nom_Fichier>", argv[0]);
		exit(EXIT_FAILURE);
	}

	bool retour = compression(argv[1], 4, 4);

	if (retour == true) {
		printf("PASSED\n");
	}

	return EXIT_SUCCESS;
}
