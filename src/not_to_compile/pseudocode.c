// pseudo code à la va très vite que je reprendrai demain mais pour l'idée générale

void ecriture_dans_le_bitstream(bitstream, huff1, huff2, bloc, difference_DC) {
    uint nb_16_zeros;
    uint nb_zeros_restants;
    uint nb_zeros_enchaines = 0;
    uint magnitude;
    uint indice;
    for k in range 8 {
        for l in range 8 {
            if (k == 0 and l == 0) {
                magnitude, indice  =  encodeDC(difference_DC);
                ecrire_bitstream(magnitude +  indice);
            }
            else {
                magnitude,  indice, nb_16_zeros, nb_zeros_restants =encodeAC(matrice[k][l], 16*nb_16_zeros+nb_zeros_restants);
                if (16*nb_16_zeros+nb_zeros_restants != 0) {
                    ecrire_bitstream(nb_16_zeros*ZRL + nb_zeros_restants + magnitude + indice);
                }
            }
    ecrire_bitstream(EOB);
}
