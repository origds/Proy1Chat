/* Descripcion de los procedimientos de lista.c
 * Autores: Oriana Gomez	09-10336
 *			Ivan Travecedo	08-11131
 */

typedef struct nodo {
  char *nombre;
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
int insertar (Lista * lista, char *nombre, Lista * asoc);
int eliminar(Lista *lista, char * nombre);
int borrar(Lista *lista);
void printListaPpal(Lista * lista);
void printListaAux(Lista * lista, char *nombre);
int buscar(Lista *lista, char * nombre);
Elemento * buscarAux(Lista *lista, char * nombre, char * segunda);
int eliminarPpalYAux(Lista * listaPpal,Lista * listaAux, char * nombre);