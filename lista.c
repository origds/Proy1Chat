#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lista.h"


void nuevaLista (Lista *lista){
  lista->ini = NULL;
  lista->fin = NULL;
  lista->tam = 0;
}

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

void printListaAuxCompleta(Lista * lista) {
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
}

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

int eliminar(Lista *lista, char * nombre){
  Elemento *sup_elemento;

  if(lista->tam == 0)
    return -1;

  sup_elemento = lista->ini;
  while(sup_elemento!=NULL){
    if(strcmp(sup_elemento->nombre,nombre)==0){
      if(sup_elemento->ant == NULL){
        lista->ini = sup_elemento->sig;
        if(sup_elemento->sig!=NULL)
          sup_elemento->sig->ant = sup_elemento->ant;
      }
      if (sup_elemento-> sig == NULL){
        if(sup_elemento->ant!=NULL)
          sup_elemento->ant->sig = NULL;
        lista->fin = sup_elemento->ant;
      }
      if(sup_elemento->ant!=NULL && sup_elemento->sig!=NULL){
        sup_elemento->ant->sig = sup_elemento->sig;
        sup_elemento->sig->ant = sup_elemento->ant;
      }
      if(sup_elemento->lista!=NULL)
        borrar(sup_elemento->lista);
      free(sup_elemento->nombre);
      free(sup_elemento);
      lista->tam--;
      return 0;
    }
    else{
      sup_elemento = sup_elemento->sig;
    }
  }
  return -1;
}

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

/*
 * return -1 ERROR
 * return 0 ENCONTRADO
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


int insertarAux(Lista *lista, char * nombreppal, char * nombreaux){
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
        return insertar(nueva, nombreaux, 0,NULL);
      }
      return insertar(elem->lista,nombreaux, 0,NULL);
    }
    else{
      elem = elem->sig;
    }
  }
  return -1;
}