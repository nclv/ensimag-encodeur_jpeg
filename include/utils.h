#ifndef _UTILS_H_
#define _UTILS_H_

/* type : const char*
 * rtype : char*
 * Permet de duppliquer une chaîne de caractères
 */
extern char *strdup(const char *str);

/* type : char*
 * rtype : void
 * Enlève l'extension d'une chaîne de caractères
 * représentant un nom de fichier
 */
extern void strip_ext(char *fname);

#endif
