/* Descripcion de los procedimientos de lista.c
 * Autores: Oriana Gomez	09-10336
 *			Ivan Travecedo	08-11131
 */

typedef struct nodo {
  char *nombre;
  int fd;
  struct ListaId *lista;
  struct nodo *sig;
  struct nodo *ant;
}Elemento;

typedef struct ListaId{
  Elemento *fin;
  Elemento *ini;
  int tam;
}Lista;

void nuevaLista (Lista *lista);
int insertar (Lista * lista, char *nombre, int fd, Lista * asoc);
int eliminar(Lista *lista, char * nombre);
int borrar(Lista *lista);
void printListaPpal(Lista * lista);
void printListaAux(Lista * lista, char *nombre);
void printListaAuxCompleta(Lista * lista);
int buscar(Lista *lista, char * nombre);
Lista * buscarPpalLista(Lista *lista, char * nombre);
int eliminarPpalYAux(Lista * listaPpal,Lista * listaAux, char * nombre);
int eliminarAux(Lista * listaPpal,Lista * listaAux, char * nombre);