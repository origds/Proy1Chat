/* Descripcion de todas las funciones de procesarInstrucciones */
#include "lista.h"

void generarInstrucciones(char* saladefault, char * user, char * instruccion);

int procesarInstrucciones (char * saladefault, char * user, char * comando, char * argumento);

int crearUsuariosSalas(char *saladefecto);

char * funcionMen(char * user);

int crearUsuario(char * nombre, char * sala, int fd);

int crearSala(char *sala);

char * funcionSalUsu(Lista *lista);

int funcionSus(char * user, char* sala);

int existeUsuario(char * nombre, char * saladefecto, int fd);