/* Archivo: lista.c
 * Autores: Oriana Gomez   09-10336
 *          Ivan Travecedo 08-11131
 * Descripcion: Implementacion de las listas para el manejo de salas y usuarios.
 *              Implementacion de las listas de hilos.
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lista.h"

/* Variable para inicializar los mutex de listas 
 * de usuarios y salas
 */
pthread_mutex_t mutex1= PTHREAD_MUTEX_INITIALIZER;

/* Procedimiento: nuevaLista
 * Descripcion: Inicializa una lista
 * Parametro: Lista que se va a inicializar
 */
void nuevaLista (Lista *lista){
  lista->ini = NULL;
  lista->fin = NULL;
  lista->tam = 0;
  lista->mutex = mutex1;
}

void nuevaListaT (ListaThread *lista){
  lista->ini = NULL;
  lista->fin = NULL;
  lista->tam = 0;
}

/* Procedimiento: printListaPPal
 * Descripcion: Imprime los nombres de salas/usuarios
 * Parametro: Lista (de usuarios o salas) de donde se
 *            va a imprimir
 */
void printListaPpal(Lista * lista){
  Elemento *elem;

  if(lista==NULL || lista->tam == 0){
    printf("No hay elementos en la lista\n");
  }
  else{
    elem = lista->ini;
    while(elem!=NULL){
      printf("Nombre: %s\n", elem->nombre);
      elem = elem ->sig;
    }
  }
}

/* Procedimiento: printListaAux
 * Descripcion: Imprime los nombres de salas/usuarios
 *              que tiene asociado una sala o un usuario
 *              en especifico
 * Parametro: Lista * lista: (de usuarios o salas) de donde se
 *                            va a imprimir
 *            char * nombre: nombre de sala/usuario al que se le
 *                           imprimiran sus salas/usuarios
 */
void printListaAux(Lista * lista, char *nombre){
  Elemento *elem;
  if(lista == NULL || lista->tam == 0){
    printf("No hay elementos en la lista\n");
  } 
  else{
    elem = lista->ini;
    while(elem!=NULL){
      if(strcmp(elem->nombre,nombre)==0){
        printListaPpal(elem->lista);
        elem = NULL;
      }
      else{
        elem = elem ->sig;
      }
    }
  }
}

/**/

/*void printListaAuxCompleta(Lista * lista) {
  Elemento * elem;
  if(lista == NULL || lista->tam == 0){
    printf("No hay elementos en la lista\n");
  }
  else { 
    elem = lista->ini;
    while(elem!=NULL){
        printf("lista aux\n");
        printListaPpal(elem->lista);
        elem = elem ->sig;
    }
  }
}*/

/* Procedimiento: insertar
 * Descripcion: Inserta un nuevo usuario/lista a las listas
 *              generales. Si es un usuario, se guarda junto
 *              al filedescriptor de su socket asociado.
 * Parametro: Lista * lista: donde se va agregar
 *            char * nombre: nombre de sala/usuario a agregar
 *            int fd: filedescriptor del usuario (si es una sala
 *            se guarda con -1)
 *            Lista * asoc: lista asociada de salas/usuarios para
 *            "nombre"
 * Retorna: -1 en caso de error
 *          0 si no hay error
 */

int insertar (Lista * lista, char *nombre, int fd, Lista * asoc){
  Elemento *elem;
  if ((elem = (Elemento *) malloc (sizeof(Elemento))) == NULL)
    return -1;
  if ((elem->nombre = (char*)malloc(sizeof(nombre)))==NULL)
    return -1;
  strcpy (elem->nombre, nombre);
  elem->lista = asoc;
  elem->fd = fd;
  if(lista->tam==0){
    elem->ant = lista->ini;
    elem->sig = lista->fin;
    lista->ini = elem;
    lista->fin = elem;
    lista->tam++;
  }
  else{
    elem->ant = NULL;
    elem->sig = lista->ini;
    lista->ini->ant = elem;
    lista->ini = elem;
    lista->tam++;
  }
  return 0;
}

/* Funcion: insertarT
 * Descripcion: Inserta un nuevo hilo a la lista con su
 *              identificador asociadao y el filedescriptor que
 *              atiende
 * Parametro: ListaThread * lista: donde se va agregar
 *            pthread_t id: id del hilo
 *            int fd: filedescriptor del usuario que le hilo
 *                    atiende
 * Retorna: -1 en caso de error
 *          0 si no hay error
 */

int insertarT (ListaThread * lista, pthread_t id, int fd){
  Thread *elem;
  if ((elem = (Thread *) malloc (sizeof(Thread))) == NULL)
    return -1;
  elem->id = id;
  elem->fd = fd;
  if(lista->tam==0){
    elem->ant = lista->ini;
    elem->sig = lista->fin;
    lista->ini = elem;
    lista->fin = elem;
    lista->tam++;
  }
  else{
    elem->ant = NULL;
    elem->sig = lista->ini;
    lista->ini->ant = elem;
    lista->ini = elem;
    lista->tam++;
  }
  return 0;
}

/* Funcion: eliminar
 * Descripcion: Elimina un usuario/sala de las listas de
 *              usuarios/salas
 * Parametro: Lista * lista: donde se va a eliminar
 *            char * nombre: nombre de la sala/usuario a eliminar
 * Retorna: -1 en caso de error
 *          0 si no hay error
 */

int eliminar(Lista *lista, char * nombre){
  Elemento *elem;

  if(lista->tam == 0)
    return -1;

  elem = lista->ini;
  while(elem!=NULL){
    if(strcmp(elem->nombre,nombre)==0){
      if(elem->ant == NULL){
        lista->ini = elem->sig;
        if(elem->sig!=NULL)
          elem->sig->ant = elem->ant;
      }
      if (elem-> sig == NULL){
        if(elem->ant!=NULL)
          elem->ant->sig = NULL;
        lista->fin = elem->ant;
      }
      if(elem->ant!=NULL && elem->sig!=NULL){
        elem->ant->sig = elem->sig;
        elem->sig->ant = elem->ant;
      }
      if(elem->lista!=NULL)
        borrar(elem->lista);
      free(elem->nombre);
      free(elem);
      lista->tam--;
      return 0;
    }
    else{
      elem = elem->sig;
    }
  }
  return -1;
}

/* Funcion: borrar
 * Descripcion: Borra una lista completa liberando memoria
 * Parametro: Lista * lista: lista a borrar
 * Retorna: -1 en caso de error
 *          0 si no hay error
 */

int borrar(Lista *lista){
  Elemento *elem;
  while (lista->ini != NULL){
    elem = lista->ini;
    lista->ini = lista->ini->sig;
    if(elem->sig!=NULL){
      elem->sig->ant=NULL;
    }
    free(elem->nombre);
    if(elem->lista!=NULL){
      borrar(elem->lista);
    }
    free(elem);
  }
}

/*int borrarT(ListaThread *lista){
  Thread *elem;
  while (lista->ini != NULL){
    elem = lista->ini;
    lista->ini = lista->ini->sig;
    if(elem->sig!=NULL){
      elem->sig->ant=NULL;
    }
    free(elem);
  }
}*/


/* Funcion: buscarPpal
 * Descripcion: Busca en la lista de usuarios/salas
 *              un usuario/sala.
 * Parametro: Lista * lista: lista en la que se buscara
 *            char * nombre: nombre de usuario/sala a buscar
 * Retorna: -1 si no lo encuentra
 *          0 si si lo encuentra
 */

int buscarPpal(Lista *lista, char * nombre){
  Elemento *elem;

  if(lista->tam == 0)
    return -1;

  elem = lista->ini;
  while(elem!=NULL){
    if(strcmp(elem->nombre,nombre)==0){
      return 0;
    }
    else{
      elem = elem->sig;
    }
  }
  return -1;
}

/* Funcion: buscarPpalLista
 * Descripcion: Busca en la lista de usuarios/salas
 *              un usuario/sala y devuelve su lista de
 *              usuarios/salas asociada
 * Parametro: Lista * lista: lista en la que se buscara
 *            char * nombre: nombre de usuario/sala a buscar
 * Retorna: Lista *: lista asociada
 */

Lista * buscarPpalLista(Lista *lista, char * nombre){
  Elemento *elem;

  if(lista->tam == 0)
    return NULL;

  elem = lista->ini;
  while(elem!=NULL){
    if(strcmp(elem->nombre,nombre)==0){
      return elem->lista;
    }
    else{
      elem = elem->sig;
    }
  }
  return NULL;
}

/* Funcion: eliminarPpalYAux
 * Descripcion: Caso1: Elimina un usuario y lo saca de la lista de usuarios
 *              de todas las salas a las que esta suscrito
 *              Caso2: Elimina una sala y la saca de todos los usuarios que
 *              estan suscritos a ella
 * Parametro: Lista * listaPpal: lista en la que se buscara inicialmente
 *            Lista * listaAux: lista en la que se buscaran las asociaciones
 *            char * nombre: nombre de usuario/sala a buscar
 * Retorna: -1 en caso de error
 *          0 en caso satisfactorio
 */

int eliminarPpalYAux(Lista * listaPpal,Lista * listaAux, char * nombre){
  Elemento * elem, * aux, *ppal;
  Lista * del;
  int elimino = 0;

  if(listaPpal->tam == 0)
    return -1;

  elem = listaPpal->ini;
  while(elem!=NULL){ // conseguir el nombre en la lista ppal
    if(strcmp(elem->nombre,nombre)==0){ //nombre ppal encontrado
      ppal = elem;

      if (elem->lista !=NULL && elem->lista->ini!=NULL){
        del = elem->lista;

        elem = elem->lista->ini; // ini de la lista auxiliar para nombre ppal
        
        aux = listaAux->ini; // ini de listaAux
        while(elem!=NULL){
          while(aux!=NULL){
            if (strcmp(elem->nombre,aux->nombre)==0){
              elimino = elimino + eliminar(aux->lista, nombre);
              elem = elem->sig;
              aux = listaAux->ini;
              break;
            }
            else{
              aux = aux->sig;
            }
          }
        }
        if(del!=NULL)
          borrar(del);
        ppal->lista = NULL;
        eliminar(listaPpal,ppal->nombre);
        return elimino;
      }
      return eliminar(listaPpal,ppal->nombre);
    }
    else{ // nombre ppal no encontrado
      elem = elem->sig;
    }
  }
  return -1;
}


/* Funcion: eliminarAux
 * Descripcion: Elimina la lista auxiliar de un usuario/sala
 * Parametro: Lista * listaPpal: lista en la que se buscara inicialmente
 *            Lista * listaAux: lista en la que se buscaran las asociaciones
 *            char * nombre: nombre de usuario/sala a buscar
 * Retorna: -1 en caso de error
 *          0 en caso satisfactorio
 */
int eliminarAux(Lista * listaPpal,Lista * listaAux, char * nombre){
  Elemento * elem, * aux, *ppal;
  Lista * del;
  int elimino = 0;

  if(listaPpal->tam == 0)
    return -1;

  elem = listaPpal->ini;
  while(elem!=NULL){ // conseguir el nombre en la lista ppal
    if(strcmp(elem->nombre,nombre)==0){ //nombre ppal encontrado
      ppal = elem;

      if (elem->lista !=NULL && elem->lista->ini!=NULL){
        del = elem->lista;

        elem = elem->lista->ini; // ini de la lista auxiliar para nombre ppal
        
        aux = listaAux->ini; // ini de listaAux
        while(elem!=NULL){
          while(aux!=NULL){
            if (strcmp(elem->nombre,aux->nombre)==0){
              elimino = elimino + eliminar(aux->lista, nombre);
              elem = elem->sig;
              aux = listaAux->ini;
              break;
            }
            else{
              aux = aux->sig;
            }
          }
        }
        if(del!=NULL)
          borrar(del);
        ppal->lista = NULL;
        return elimino;
      }
      return -1;
    }
    else{ // nombre ppal no encontrado
      elem = elem->sig;
    }
  }
  return -1;
}

/* Funcion: insertarAux
 * Descripcion: Inserta en la lista auxiliar de un usuario/sala
 * Parametro: Lista * lista: lista en la que se buscara inicialmente
 *            char * nombreppal: nombre a buscar e insertar en su lista asociada
 *            char * nombreaux: nombre a ser introducido en la lista auxiliar
 *            int fd: fildescriptor del usuario en caso de estar introduciendo
 *                    un usuario a una sala
 * Retorna: -1 en caso de error
 *          0 en caso satisfactorio
 */
int insertarAux(Lista *lista, char * nombreppal, char * nombreaux, int fd){
  Elemento *elem;
  Lista * nueva;

  if(lista->tam == 0)
    return -1;

  elem = lista->ini;
  while(elem!=NULL){
    if(strcmp(elem->nombre,nombreppal)==0){
      if (elem->lista == NULL){
        nueva = (Lista *) malloc (sizeof(Lista));
        nuevaLista(nueva);
        elem->lista = nueva;
        return insertar(nueva, nombreaux, fd, NULL);
      }
      else{
        if(buscarPpal(elem->lista,nombreaux)!=0)
          return insertar(elem->lista,nombreaux, fd, NULL);
        return -1;
      }
    }
    else{
      elem = elem->sig;
    }
  }
  return -1;
}