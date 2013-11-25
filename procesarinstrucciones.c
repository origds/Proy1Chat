#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include "procesarinstrucciones.h"
//#include "lista.h"

Lista * usuarios;
Lista * salas;

void generarInstrucciones(char * user, char * instruccion){

  char * token;
  char * comando;
  char * argumento;

  comando = (char *)malloc(sizeof(char)*3);
  argumento = (char *)malloc(sizeof(char)*100);

  token = strtok (instruccion,"\n");
  token = strtok(token," ");
  if(token!=NULL && (strcmp(token,"men")==0 || strcmp(token,"sus")==0 || strcmp(token,"cre")==0 || strcmp(token,"eli")==0)){
     strcpy(comando, token);
     token = strtok (NULL,"\n");
     strcpy(argumento, token);
  } else {
     strcpy(comando, token);
     strcpy(argumento, " ");
  }  
  procesarInstrucciones(user, comando, argumento);
}

char * funcionSalUsu(Lista *lista){
	Elemento * elem;
	int tam;
	char * elementos;

	tam = lista->tam;
	if (tam>0){
		elementos = (char *)malloc(sizeof(char)*(20*tam));
		elem = lista->ini;
		strcpy(elementos, elem->nombre);
		elem = elem->sig;
		while (elem!=NULL) {
			strcat(elementos, " ");
			strcat(elementos, elem->nombre);
			elem = elem->sig;
		}
	} else {
		elementos = (char *)malloc(sizeof(char)*(1));
		strcpy(elementos, " ");
	}

	return elementos;
}

int procesarInstrucciones (char * user, char * comando, char * argumento){
	if (strcmp(comando,"sal")==0 && strcmp(argumento," ")==0) {
		char * listasalas = funcionSalUsu(salas);
		printf("Salas: %s\n", listasalas);
		return 0;
		//falta funcionMen
	} else if (strcmp(comando,"usu")==0 && strcmp(argumento," ")==0) {
		char * listausuarios;
		listausuarios = funcionSalUsu(usuarios);
		printf("Usuarios: %s\n", listausuarios);
		return 0;
	} else if (strcmp(comando,"sus")==0 && strcmp(argumento," ")!=0) {
		return funcionSus(user, argumento);
	} else if (strcmp(comando,"des")==0 && strcmp(argumento," ")==0) {
		return funcionDes(user);
	} else if (strcmp(comando,"cre")==0 && strcmp(argumento," ")!=0) {
		return funcionCre(argumento);
	} else if (strcmp(comando,"eli")==0 && strcmp(argumento," ")!=0) {
		return funcionEli(argumento);
	}
}	

int crearUsuariosSalas(char *saladefecto) {
	int a;
	usuarios = (Lista *) malloc (sizeof(Lista));
	nuevaLista(usuarios);
	salas = (Lista *) malloc (sizeof(Lista));
	nuevaLista(salas);
	a = insertar(salas, saladefecto, NULL);
}

int crearUsuario(Lista *lista, char * nombre, char * sala){
	int a, b;
	//en menuschat debo verificar que si el nombre ya existe entonces debe ingresar otro.
	Lista *salasuser;
	salasuser = (Lista *) malloc (sizeof(Lista));
	nuevaLista(salasuser);
	a = insertar(salasuser, sala, NULL);
	nuevaLista(usuarios);
	b = insertar(lista, nombre, salasuser);
	return (a+b);
}

/*char * funcionMen(char * user) {
	Elemento * usuario, salasuser, del;
	char * sala;
	int borrado1, borrado2;
	usuario = buscarPpal(usuarios, user);
	salasuser = usuario->lista->ini;
	while (salasuser!=NULL){
		sala = salauser->nombre;
		del = buscarPpal(salas, sala);
		if (del!=NULL) {
			borrado1 = eliminar(del->lista, user);
			if (borrado1==0) {
				salauser = salauser->sig;
			} else {
				return -1;
			} 
		} else {
			return -1;
		}
	}
	borrado2 = eliminar(usuarios, user);
	return (borrado1+borrado2); 
}*/

int funcionSus(char * user, char* sala) {
	int a, b;
	//a la lista de salas le agregamos el usuario que se suscribe a esa sala
	a = insertarAux(salas, sala, user);
	//a la lista de usuarios le agregamos la sala a la que se suscribe el usuario
	b = insertarAux(usuarios, user, sala);
	return (a+b);
}

int funcionDes(char * user){
	return eliminarPpalYAux(usuarios,salas,user);
}

int funcionCre(char * nombresala){
	return insertar(salas, nombresala, NULL);
}

int funcionEli(char * sala){
	return eliminarPpalYAux(salas,usuarios,sala);
}