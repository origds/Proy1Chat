/* Descripcion de todas las funciones de procesarInstrucciones */

void generarInstrucciones(char * instruccion);

int procesarInstrucciones (char * user, char * comando, char * argumento);

int crearUsuariosSalas(char *saladefecto);

char * funcionMen(char * user);

void crearUsusario(Lista *lista, char * nombre, char * sala);

int crearSala(char *sala);

char * funcionSalUsu(Lista *lista);

int funcionSus(char * user, char* sala);

int funcionDes(char * user);

int funcionCre(char * nombresala);

int funcionEli(char * sala);