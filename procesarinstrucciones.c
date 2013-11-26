#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include "procesarinstrucciones.h"
//#include "lista.h"

Lista * usuarios;
Lista * salas;

/* Funcion que separa las instrucciones recibidas del cliente en comando y argumento
   void */

void generarInstrucciones(char * saladefault, char * user, char * instruccion){

  char * token;
  char * comando;
  char * argumento;

  comando = (char *)malloc(sizeof(char)*3);
  argumento = (char *)malloc(sizeof(char)*100);

  token = strtok (instruccion,"\n");
  token = strtok(token," ");
  if(token!=NULL && (strcmp(token,"men")==0 || strcmp(token,"sus")==0 || strcmp(token,"cre")==0 
  	|| strcmp(token,"eli")==0 || strcmp(token,"fue")==0)) {
     strcpy(comando, token);
     token = strtok (NULL,"\n");
     strcpy(argumento, token);
  } else {
     strcpy(comando, token);
     strcpy(argumento, " ");
  }  
  procesarInstrucciones(saladefault, user, comando, argumento);
}

/* Funcion que responde a sus y usu. Devuelve la lista de usuarios o salas
   Retorna: char* */

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

/* Funcion que inicializa la lista de salas y usuarios
   Retorna: int */

int crearUsuariosSalas(char *saladefecto) {
	int a;
	usuarios = (Lista *) malloc (sizeof(Lista));
	nuevaLista(usuarios);
	salas = (Lista *) malloc (sizeof(Lista));
	nuevaLista(salas);
	a = insertar(salas, saladefecto, NULL);
}


/* Funcion que chequea si el usuario ya existe 
   Return: int*/

int existeUsuario(char * nombre, char * saladefecto) {
	if (buscarPpal(usuarios, nombre)!=0) {
		crearUsuario(nombre, saladefecto);
		printf("USUARIOOOOS\n");
		printListaPpal(usuarios);
		printf("SALAAAS\n");
		printListaPpal(salas);
		return 0;
	} else {
		return -1;
	}
}

/* Funcion que responde a cuando se recibe un nuevo cliente
   Retorna: int */


int crearUsuario(char * nombre, char * sala){
	int a;
	//en menuschat debo verificar que si el nombre ya existe entonces debe ingresar otro.
	a = insertar(usuarios, nombre, NULL);
	//printf("IMPRIMIENDO USUARIOS\n");
	//printListaPpal(usuarios);
	if (a==0) {
		return funcionSus(nombre, sala);
	}
	printf("No pude crear usuario\n");
	return -1;
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

/* Funcion que responde a sus. Suscribe un usuario a una sala
   Retorna: int */

int funcionSus(char * user, char* sala) {
	int a, b;
	//a la lista de salas le agregamos el usuario que se suscribe a esa sala
	a = insertarAux(salas, sala, user);
	//a la lista de usuarios le agregamos la sala a la que se suscribe el usuario
	b = insertarAux(usuarios, user, sala);
	return (a+b);
}

/* Funcion que procesa las instrucciones recibidas del cliente 
	Retorna: int */

int procesarInstrucciones (char * saladefault, char * user, char * comando, char * argumento){
	int a;
	if (strcmp(comando,"sal")==0 && strcmp(argumento," ")==0) {
		char * listasalas;
		listasalas = funcionSalUsu(salas);
		//printf("Salas: %s\n", listasalas);
		printf("USUARIOOOOS\n");
		printListaPpal(usuarios);
		printf("LISTA AUX\n");
		printListaAuxCompleta(usuarios);
		printf("SALAAAS\n");
		printListaPpal(salas);
		printf("LISTA AUX\n");
		printListaAuxCompleta(salas);
		return 0;
		//falta funcionMen
	} else if (strcmp(comando,"usu")==0 && strcmp(argumento," ")==0) {
		char * listausuarios;
		listausuarios = funcionSalUsu(usuarios);
		//printf("Usuarios: %s\n", listausuarios);
		printf("USUARIOOOOS\n");
		printListaPpal(usuarios);
		printf("LISTA AUX\n");
		printListaAuxCompleta(usuarios);
		printf("SALAAAS\n");
		printListaPpal(salas);
		printf("LISTA AUX\n");
		printListaAuxCompleta(salas);
		return 0;
	} else if (strcmp(comando,"sus")==0 && strcmp(argumento," ")!=0) {
		a = funcionSus(user, argumento);
		printf("USUARIOOOOS\n");
		printListaPpal(usuarios);
		printf("LISTA AUX\n");
		printListaAuxCompleta(usuarios);
		printf("SALAAAS\n");
		printListaPpal(salas);
		printf("LISTA AUX\n");
		printListaAuxCompleta(salas);
		return a;
	} else if (strcmp(comando,"des")==0 && strcmp(argumento," ")==0) {
		a = eliminarPpalYAux(usuarios,salas,user);
		printf("USUARIOOOOS\n");
		printListaPpal(usuarios);
		printf("LISTA AUX\n");
		printListaAuxCompleta(usuarios);
		printf("SALAAAS\n");
		printListaPpal(salas);
		printf("LISTA AUX\n");
		printListaAuxCompleta(salas);
		return a;
	} else if (strcmp(comando,"cre")==0 && strcmp(argumento," ")!=0) {
		a =  insertar(salas, argumento, NULL);
		printf("USUARIOOOOS\n");
		printListaPpal(usuarios);
		printf("LISTA AUX\n");
		printListaAuxCompleta(usuarios);
		printf("SALAAAS\n");
		printListaPpal(salas);
		printf("LISTA AUX\n");
		printListaAuxCompleta(salas);
		return a;
	} else if (strcmp(comando,"eli")==0 && strcmp(argumento," ")!=0) {
		if (strcmp(argumento, saladefault)==0) {
			printf("No se puede eliminar la sala por defecto\n");
		} else {
			a =  eliminarPpalYAux(salas,usuarios,argumento);
		}
		printf("USUARIOOOOS\n");
		printListaPpal(usuarios);
		printf("LISTA AUX\n");
		printListaAuxCompleta(usuarios);
		printf("SALAAAS\n");
		printListaPpal(salas);
		printf("LISTA AUX\n");
		printListaAuxCompleta(salas);
		return a;
	} else if (strcmp(comando,"fue")==0 && strcmp(argumento," ")!=0) {
		a =  eliminarPpalYAux(usuarios,salas,argumento);
		printf("USUARIOOOOS\n");
		printListaPpal(usuarios);
		printf("LISTA AUX\n");
		printListaAuxCompleta(usuarios);
		printf("SALAAAS\n");
		printListaPpal(salas);
		printf("LISTA AUX\n");
		printListaAuxCompleta(salas);
		return a;
	}
}	