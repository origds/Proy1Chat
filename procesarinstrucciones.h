/* Descripcion de todas las funciones de procesarInstrucciones */
#include "lista.h"

void generarInstrucciones(char * saladefault, char * user, char * instruccion, int fd);
int procesarInstrucciones (char * saladefault, char * user, char * comando, char * argumento, int fd);
int crearUsuariosSalas(char *saladefecto);
void funcionMen(char * user, char * mensaje);
int crearUsuario(char * nombre, char * sala, int fd);
int crearSala(char *sala);
void funcionSalUsu(Lista *lista,int fd);
int funcionSus(char * user, char* sala, int fd);
int existeUsuario(char * nombre, char * saladefecto, int fd);
void actualizarFdSockets(int fdsocket, fd_set * descriptoresLectura);
int tamanoUsuarios();