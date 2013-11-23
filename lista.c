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
  printf("Entra borrar\n");
  while (lista->ini != NULL){
    printf("entra while borrar\n");
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

void printLista(Lista * lista){
  Elemento *elem;
  if(lista->tam == 0){
    printf("No hay elementos en la lista\n");
    exit(0);
  }
  else{
    elem = lista->ini;
    printf("Elementos:\n");
    while(elem!=NULL){
      printf("Nombre:%s\n", elem->nombre);
      elem = elem ->sig;
    }
  }
}

/*int main(){
  Lista * lista;
  lista = (Lista *) malloc (sizeof(Lista));
  nuevaLista(lista);
  insertar(lista,"elem1", NULL);
  insertar(lista,"elem2", NULL);
  insertar(lista,"elem3", NULL);
  insertar(lista,"elem4", NULL);
  printLista(lista);
  printf("Va a entrar a borrar\n");
  eliminar(lista,"elem2");
  printLista(lista);
  eliminar(lista,"elem3");
  printLista(lista);
  eliminar(lista,"elem1");
  printLista(lista);
  eliminar(lista,"elem4");
  printLista(lista);

  borrar(lista);
  free(lista);
}*/