// pseudo code à la va très vite que je reprendrai demain mais pour l'idée générale

void ecriture_dans_le_bitstream(bitstream, huff1, huff2, bloc, difference_DC) {
    uint nb_zeros_enchaines = 0;
    for k in range 8 {
        for l in range 8 {
            if (k == 0 and l == 0) {
                // ecrit dans le bitstream magnitude + indice
                encodeDC(difference_DC);
            }
            else {
                if (matrice[k][l] == 0) {
                    // incrementer les zeros a la suite
                    nb_zeros_enchaines++;
                }
                else {
                    /* ecrit dans le bitstream le bon nb de ZRL, le nb de zeros restants,
                       la magnitude et l'indice */
                    encodeAC(matrice[k][l], nb_zeros_enchaines);
                    // reset le compteur de zeros  enchaines
                    nb_zeros_enchaines = 0;
                }
            }
    ecrire_bitstream(EOB);
}
