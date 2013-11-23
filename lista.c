#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lista.h"


void nuevaLista (Lista *lista){
  lista->ini = NULL;
  lista->fin = NULL;
  lista->tam = 0;
}

int insertar (Lista * lista, char *nombre, Lista * asoc){
  Elemento *elem;
  if ((elem = (Elemento *) malloc (sizeof(Elemento))) == NULL)
    return -1;
  if ((elem->nombre = (char*)malloc(sizeof(nombre)))==NULL)
    return -1;
  strcpy (elem->nombre, nombre);
  elem->lista = asoc;
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
      printf("entra borrar recursivo\n");
    }
    free(elem);
  }
}

void printListaPpal(Lista * lista){
  Elemento *elem;

  if(lista==NULL || lista->tam == 0){
    printf("No hay elementos en la lista\n");
    exit(0);
  }
  else{
    elem = lista->ini;
    while(elem!=NULL){
      printf("Nombre:%s\n", elem->nombre);
      elem = elem ->sig;
    }
  }
}

void printListaAux(Lista * lista, char *nombre){
  Elemento *elem;
  if(lista->tam == 0){
    printf("No hay elementos en la lista\n");
    exit(0);
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

int buscarAux(Lista *lista, char * nombre, char * segunda){
  Elemento *elem;

  if(lista->tam == 0)
    return -1;

  elem = lista->ini;
  while(elem!=NULL){
    if(strcmp(elem->nombre,nombre)==0){
      return buscarPpal(elem->lista,segunda);
    }
    else{
      elem = elem->sig;
    }
  }
  return -1;
}

/*int main(){
  Lista * usuarios;
  usuarios = (Lista *) malloc (sizeof(Lista));
  
  Lista * salasMary;
  salasMary = (Lista *) malloc (sizeof(Lista));

  Lista * salasCarla;
  salasCarla = (Lista *) malloc (sizeof(Lista));

  Lista * salasOriana;
  salasOriana = (Lista *) malloc (sizeof(Lista));

  nuevaLista(usuarios);
  nuevaLista(salasMary);
  nuevaLista(salasCarla);
  nuevaLista(salasOriana);

  insertar(salasMary,"sala 1", NULL);
  insertar(salasMary,"sala 2", NULL);
  insertar(salasMary,"sala 3", NULL);

  insertar(salasCarla,"sala 4", NULL);
  insertar(salasCarla,"sala 5", NULL);

  insertar(salasOriana,"sala 1", NULL);
  insertar(salasOriana,"sala 2", NULL);
  insertar(salasOriana,"sala 3", NULL);
  insertar(salasOriana,"sala 4", NULL);
  insertar(salasOriana,"sala 5", NULL);


  insertar(usuarios,"Mary", salasMary);
  insertar(usuarios,"Ivan", NULL);
  insertar(usuarios,"Carla", salasCarla);
  insertar(usuarios,"Oriana", salasOriana);
  
  printf("Usuarios:\n");
  printListaPpal(usuarios);

  printf("Salas de Carla:\n");
  printListaAux(usuarios,"Carla");

  printf("Salas de Mary:\n");
  printListaAux(usuarios,"Mary");

  printf("Salas de Orian:\n");
  printListaAux(usuarios,"Oriana");  

  printf("Salas de Ivan:\n");
  printListaAux(usuarios,"Ivan");

  borrar(usuarios);
  free(usuarios);
}*/
