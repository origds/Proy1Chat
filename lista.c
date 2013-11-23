#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void nuevaLista (Lista *lista){
  lista->ini = NULL;
  lista->fin = NULL;
  lista->tam = 0;
}

int insertarVacia (Lista * lista, char *nombre, Lista * asoc){
  Elemento *elem;
  if ((elem = (Elemento *) malloc (sizeof(Elemento))) == NULL)
    return -1;
  if ((elem->nombre = (char*)malloc(sizeof(nombre)))==NULL)
    return -1;
  strcpy (elem->nombre, nombre);
  elem->lista = asoc;
  elem->ant = lista->ini;
  elem->sig = lista->fin;
  lista->ini = elem;
  lista->fin = elem;
  lista->tam++;
  return 0;
}

int insertarNoVacia(Lista * lista, char *nombre, Lista * asoc){
  Elemento *elem;
  if ((elem = (Elemento *) malloc (sizeof(Elemento))) == NULL)
    return -1;
  if ((elem->nombre = (char*)malloc(sizeof(nombre)))==NULL)
    return -1;  
  strcpy (elem->nombre, nombre);
  elem->lista = asoc;
  elem->ant = NULL;
  elem->sig = lista->ini;
  lista->ini->ant = elem;
  lista->ini = elem;
  lista->tam++;
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
      //llamar a una funcion para hacer free de la lista del elemento
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
  printf("Entra borrar");
  while (lista->ini != NULL){
    printf("entra while borrar");
    elem = lista->ini;
    lista->ini = lista->ini->sig;
    elem->sig->ant=NULL;
    free(elem->nombre);
    free(elem->sig);
    free(elem->ant);
    if(elem->lista!=NULL){
      borrar(elem->lista);
      printf("entra borrar recursivo");
    }
    free(elem);
  }
  free(lista);
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

int main(){
  Lista * lista;
  lista = (Lista *) malloc (sizeof(Lista));
  nuevaLista(lista);
  insertarVacia(lista,"elem1", NULL);
  insertarNoVacia(lista,"elem2", NULL);
  insertarNoVacia(lista,"elem3", NULL);
  insertarNoVacia(lista,"elem4", NULL);
  printLista(lista);
  printf("Va a entrar a borrar");
  /*eliminar(lista,"elem2");
  printLista(lista);
  eliminar(lista,"elem3");
  printLista(lista);
  eliminar(lista,"elem1");
  printLista(lista);
  eliminar(lista,"elem4");
  printLista(lista);*/

  borrar(lista);
}