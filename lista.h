/* Descripcion de los procedimientos de lista.c
 * Autores: Oriana Gomez	09-10336
 *			Ivan Travecedo	08-11131
 * Descripcion:
 *              Para la lista de salas, cada sala tiene asociada una lista de los
 *              usuarios que estan suscritas a ella
 *              Para la lista de usuarios, cada usuario tiene asociada la lista
 *              de salas a las que esta suscrito
 */

 #include <pthread.h>


typedef struct nodo {
  char *nombre;
  int fd;
  struct ListaId *lista;
  struct nodo *sig;
  struct nodo *ant;
}Elemento;

typedef struct ListaId {
  Elemento *fin;
  Elemento *ini;
  pthread_mutex_t mutex;
  int tam;
}Lista;

typedef struct Thread {
  pthread_t id;
  int fd;
  struct Thread *sig;
  struct Thread *ant;
}Thread;

typedef struct ListaThread {
  Thread *fin;
  Thread *ini;
  int tam;
}ListaThread;

void nuevaLista (Lista *lista);
int insertar (Lista * lista, char *nombre, int fd, Lista * asoc);
int eliminar(Lista *lista, char * nombre);
int borrar(Lista *lista);
void printListaPpal(Lista * lista);
void printListaAux(Lista * lista, char *nombre);
int buscar(Lista *lista, char * nombre);
Lista * buscarPpalLista(Lista *lista, char * nombre);
int eliminarPpalYAux(Lista * listaPpal,Lista * listaAux, char * nombre);
int eliminarAux(Lista * listaPpal,Lista * listaAux, char * nombre);
int insertarAux(Lista *lista, char * nombreppal, char * nombreaux, int fd);

// para hilos
void nuevaListaT (ListaThread *lista);
int insertarT (ListaThread * lista, pthread_t id, int fd);