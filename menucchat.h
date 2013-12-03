/* Archivo: lista.c
 * Autores: Oriana Gomez   09-10336
 *          Ivan Travecedo 08-11131
 * Descripcion de los procedimiento de menucchat */

#include <pthread.h>

extern char *IP;
extern int puerto;
extern char *user;
extern char *archivo;

char asignarParametros(char *param[], int tam);

void menucchat(int argc, char *argv[]);