# Encodeur JPEG

Les consignes du projet sont [ici](https://formationc.pages.ensimag.fr/projet/jpeg/jpeg/).

Pour gérer le projet voir [ici](https://gitlab.ensimag.fr/vincentn/encodeur-jpeg/-/boards).

```bash
alias valgrind-o="valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind-out.txt"

compare -verbose -metric RMSE images/gris.jpg images/my-gris.jpg difference.jpg
compare -verbose -metric MAE images/gris.jpg images/my-gris.jpg difference.jpg

compare -verbose -fuzz 5% images/gris.jpg images/my-gris.jpg difference.jpg
```

