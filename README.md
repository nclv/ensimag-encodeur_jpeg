# Encodeur JPEG

Les consignes du projet sont [ici](https://formationc.pages.ensimag.fr/projet/jpeg/jpeg/).

Pour gérer le projet voir [ici](https://gitlab.ensimag.fr/vincentn/encodeur-jpeg/-/boards).

Quelques commandes utiles...
```bash
alias valgrind-o="valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind-out.txt"

compare -verbose -metric RMSE images/gris.jpg images/my-gris.jpg difference.jpg
compare -verbose -metric MAE images/gris.jpg images/my-gris.jpg difference.jpg

compare -verbose -fuzz 5% images/gris.jpg images/my-gris.jpg difference.jpg

time ./bin/ppm2jpeg images/complexite.pgm > /dev/null
```

Ce que nous avons implémenté:
 - [x] Traitement à la volée des MCU et des DU. Nous avons ainsi une empreinte mémoire très faible.
 - [x] Implémentation d'un algorithme de DCT 2D rapide (AAN) avec représentation en nombre à virgule fixe. Nous avons uniquement des opérations sur des entiers.
 - [x] Implémentation de l'algorithme de Huffman donnant les codes canoniques. Ce dernier remplace l'arbre de Huffman que nous avions aussi implémenté. On ne stocke pas un array de 0 et de 1 mais un entier sur 32 bits. Il serait intéressant d'implémenter une table de hachage permettant de renvoyer, pour une valeur à encoder donnée, le code ainsi que sa longueur en bits.
 - [x] Implémentation de l'écriture dans un fichier. On reçoit une suite de bits à regrouper en bytes. Ils sont stockés puis écrit dans le fichier binaire avec un _unique_ (buffer statique de 2048 bytes adapté à la plupart des images) appel à `fwrite` (meilleur gestion mémoire). Il serait envisageable de calculer la taille du buffer en fonction du taux de compression de l'image d'entrée.
 - [x] Gestion d'arguments en ligne de commande avec `argp`. Implémentation d'un mode verbose avec une variable globale.
 - [ ] Codes d'erreur personnalisés.

Outils:
 - Compilation de l'exécutable final avec `clang` ou `gcc` avec les flags adaptés,
 - Tests unitaires de nos modules,
 - Utilisation de `valgrind`, `gprof`, `strace`, `cppcheck` et `flawfinder`

Ce qui reste à corriger:
 - Nous rencontrons un problème lors du traitement de l'image zig-zag en 1x4,1x2,1x2.