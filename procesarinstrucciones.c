/* Archivo: procesarinstrucciones.c
 * Autores: Oriana Gomez   09-10336
 *          Ivan Travecedo 08-11131
 * Descripcion: Implementacion de las funciones dedicadas al manejo
 *              de las peticiones enviadas por los usuarios
 */



#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/select.h>
#include <unistd.h>
#include "procesarinstrucciones.h"

Lista * usuarios; //lista de usuarios
Lista * salas; // lista de salas

// devuelve el tamano de la lista de usuarios
int tamanoUsuarios(){
  return usuarios->tam;
}

/* Funcion que separa las instrucciones recibidas del 
 * cliente en comando y argumento y manda a procesar
 */

void generarInstrucciones(char * saladefault, char * user, char * instruccion, int fd){

  char * token;
  char * comando;
  char * argumento;

  comando = (char *)malloc(sizeof(char)*3);
  argumento = (char *)malloc(sizeof(char)*100);

  token = strtok (instruccion,"\n");
  token = strtok(token," ");

  if(token!=NULL && (strcmp(token,"men")==0 || strcmp(token,"sus")==0 || strcmp(token,"cre")==0 
  	|| strcmp(token,"eli")==0 || strcmp(token,"fue")==0)){
    strcpy(comando, token);
    if(strcmp(comando,"fue")!=0){
      token = strtok (NULL,"\n");
      strcpy(argumento, token);
    }
    else{
      strcpy(argumento, user);
    }
  }
  else {
    strcpy(comando, token);
    strcpy(argumento, " ");
  }
	printf("El comando es: %s\n", comando);
	printf("El argumento es: %s\n", argumento);
  procesarInstrucciones(saladefault, user, comando, argumento, fd);
}

/* Funcion que responde a sus y usu. Devuelve la lista de usuarios o salas
   Retorna: char*
   Opcion: 0 usuarios / 1 salas

 */

void funcionSalUsu(Lista *lista,int fd){
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
    if (send(fd,elementos,strlen(elementos),0)==-1){
      printf("No pudo enviarse lista usuarios/salas al cliente\n");
    }  
  }
}

/* Funcion que inicializa la lista de salas y usuarios
   Retorna: int */

int crearUsuariosSalas(char *saladefecto) {
	int a;
	usuarios = (Lista *) malloc (sizeof(Lista));
	nuevaLista(usuarios);
	salas = (Lista *) malloc (sizeof(Lista));
	nuevaLista(salas);
	a = insertar(salas, saladefecto, 0,NULL);
}


/* Funcion que chequea si el usuario ya existe 
   Return: int*/

int existeUsuario(char * nombre, char * saladefecto, int fd){
	if (buscarPpal(usuarios, nombre)!=0) {
		crearUsuario(nombre, saladefecto, fd);
		return 0;
  }
	return -1;
}

/* Funcion que responde a cuando se recibe un nuevo cliente
   Retorna: int */
int crearUsuario(char * nombre, char * sala, int fd){
	int a;
	a = insertar(usuarios, nombre, fd, NULL);
	if (a==0) {
		return funcionSus(nombre, sala, fd);
	}
	printf("Error creando usuario: %s\n",nombre);
	return -1;
}

/* Funcion Mensaje que retorna la lista de fd a la que se enviara el mensaje */

void funcionMen(char * user, char * mensaje){
  Elemento * sala, * elem;
  Lista * salasUsuario;
  Lista * usuariosEnSala;
  char * enviar;
  enviar = (char *)malloc(sizeof(char)*(sizeof(user)+sizeof(mensaje)+7));
  bzero(enviar,strlen(enviar));
  strcpy(enviar,user);
  strcat(enviar, " dice: ");
  strcat(enviar, mensaje);
  salasUsuario = buscarPpalLista(usuarios, user);
  if (salasUsuario!=NULL){
    sala = salasUsuario->ini;
    while(sala!=NULL){
      usuariosEnSala = buscarPpalLista(salas,sala->nombre);
      if(usuariosEnSala!=NULL){
        elem = usuariosEnSala->ini;
        while(elem!=NULL){
          if (send(elem->fd,enviar,strlen(enviar),0)==-1){
            printf("No pudo enviarse el nombreuser al servidor\n");
          }
          elem=elem->sig;
        }
      }
      sala = sala->sig;
    }
  }
}

/* Funcion que responde a sus. Suscribe un usuario a una sala
   Retorna: int */

int funcionSus(char * user, char* sala, int fd){
	int a, b;
	//a la lista de salas le agregamos el usuario que se suscribe a esa sala
	a = insertarAux(salas, sala, user, fd);
	//a la lista de usuarios le agregamos la sala a la que se suscribe el usuario
	if (a==0)
    b = insertarAux(usuarios, user, sala, -1); // le pongo -1 porque las salas no tienen asociado un fd
  else
    return -1;
	return b;
}


/* Funcion que responde a fue para que el cliente salga del chat
 * Retorna: int 
 */

int respuestaAutomatica(int fd, char * cmd){
  if (send(fd,cmd,strlen(cmd),0)==-1){
    printf("No pudo enviarse el nombreuser al servidor\n");
  }
}



/* Funcion que procesa las instrucciones recibidas del cliente y manda un
 * mensaje de acuerdo al resultado de la operacion
	Retorna: int */

int procesarInstrucciones (char * saladefault, char * user, char * comando, char * argumento, int fd){
	int a = 0;
  int insertada = -1;
  int error;
	printf("Procesando instruccion\n");

	if (strcmp(comando,"sal")==0 && strcmp(argumento," ")==0) {
    error = pthread_mutex_lock(&salas->mutex);
    if (error != 0) {
      errno = error;
      perror("Error intentando entrar a la lista de salas lock: ");
      exit(-1);
    }
		funcionSalUsu(salas,fd);
    error = pthread_mutex_unlock(&salas->mutex);
    if (error != 0) {
      errno = error;
      perror("Error intentando salir de la lista de salas unlock: ");
      exit(-1);
    }
	}
  else if (strcmp(comando,"usu")==0 && strcmp(argumento," ")==0) {
    error = pthread_mutex_lock(&usuarios->mutex);
    if (error != 0) {
      errno = error;
      perror("Error intentando entrar a la lista de usuarios lock: ");
      exit(-1);
    }    
    funcionSalUsu(usuarios,fd);
    error= pthread_mutex_unlock(&usuarios->mutex);
    if (error != 0) {
      errno = error;
      perror("Error intentando salir de la lista de usuarios unlock: ");
      exit(-1);
    }    
	}
  else if (strcmp(comando,"sus")==0 && strcmp(argumento," ")!=0) {
    error = pthread_mutex_lock(&usuarios->mutex);
    if (error != 0) {
      errno = error;
      perror("Error intentando entrar a la lista de usuarios lock: ");
      exit(-1);
    }
    error = pthread_mutex_lock(&salas->mutex);
    if (error != 0) {
      errno = error;
      perror("Error intentando entrar a la lista de salas lock: ");
      exit(-1);
    }
		a = funcionSus(user, argumento,fd);
    error = pthread_mutex_unlock(&salas->mutex);
    if (error != 0) {
      errno = error;
      perror("Error intentando salir de la lista de salas unlock: ");
      exit(-1);
    }
    error = pthread_mutex_unlock(&usuarios->mutex);
    if (error != 0) {
      errno = error;
      perror("Error intentando salir de la lista de usuarios unlock: ");
      exit(-1);
    } 
    if(a==0)
      respuestaAutomatica(fd,"sus");
    else
      respuestaAutomatica(fd,"suserror");
	}
  else if (strcmp(comando,"des")==0 && strcmp(argumento," ")==0) {
    error = pthread_mutex_lock(&usuarios->mutex);
    if (error != 0) {
      errno = error;
      perror("Error intentando entrar a la lista de salas lock: ");
      exit(-1);
    }
    error = pthread_mutex_lock(&salas->mutex);
    if (error != 0) {
      errno = error;
      perror("Error intentando entrar a la lista de salas lock: ");
      exit(-1);
    }    
    a = eliminarAux(usuarios,salas,user);
    error = pthread_mutex_unlock(&salas->mutex);
    if (error != 0) {
      errno = error;
      perror("Error intentando salir de la lista de salas unlock: ");
      exit(-1);
    }
    error = pthread_mutex_unlock(&usuarios->mutex);
    if (error != 0) {
      errno = error;
      perror("Error intentando salir de la lista de usuarios unlock: ");
      exit(-1);
    }
    if(a==0)
      respuestaAutomatica(fd,"des");
    else
      respuestaAutomatica(fd,"deserror");
	}
  else if (strcmp(comando,"cre")==0 && strcmp(argumento," ")!=0) {
    error = pthread_mutex_lock(&salas->mutex);
    if (error != 0) {
      errno = error;
      perror("Error intentando entrar a la lista de salas lock: ");
      exit(-1);
    }
    a = buscarPpal(salas, argumento);
    if(a!=0)
		  insertada = insertar(salas, argumento, -1, NULL);
    error = pthread_mutex_unlock(&salas->mutex);
    if (error != 0) {
      errno = error;
      perror("Error intentando salir de la lista de salas unlock: ");
      exit(-1);
    }
    if (insertada==0)
      respuestaAutomatica(fd,"cre");
    else
      respuestaAutomatica(fd,"creerror");
	}
  else if (strcmp(comando,"eli")==0 && strcmp(argumento," ")!=0) {
		if (strcmp(argumento, saladefault)==0) {
			printf("No se puede eliminar la sala por defecto\n");
      respuestaAutomatica(fd,"elidefecto");
		} else {
      error = pthread_mutex_lock(&usuarios->mutex);
      if (error != 0) {
        errno = error;
        perror("Error intentando entrar a la lista de usuarios lock: ");
        exit(-1);
      }
      error = pthread_mutex_lock(&salas->mutex);
      if (error != 0) {
        errno = error;
        perror("Error intentando entrar a la lista de salas lock: ");
        exit(-1);
      }      
			a = eliminarPpalYAux(salas,usuarios,argumento);
      error = pthread_mutex_unlock(&salas->mutex);
      if (error != 0) {
        errno = error;
        perror("Error intentando salir de la lista de salas unlock: ");
        exit(-1);
      }
      error = pthread_mutex_unlock(&usuarios->mutex);
      if (error != 0) {
        errno = error;
        perror("Error intentando salir de la lista de usuarios lock: ");
        exit(-1);
      }
      if(a==0)
        respuestaAutomatica(fd,"eli");
      else
        respuestaAutomatica(fd,"elierror");
		}
	}
  else if (strcmp(comando,"fue")==0 && strcmp(argumento," ")!=0) {
    error = pthread_mutex_lock(&usuarios->mutex);
    if (error != 0) {
      errno = error;
      perror("Error intentando entrar a la lista de usuarios lock: ");
      exit(-1);
    }
    error = pthread_mutex_lock(&salas->mutex);
    if (error != 0) {
      errno = error;
      perror("Error intentando entrar a la lista de salas lock: ");
      exit(-1);
    }    
		a = eliminarPpalYAux(usuarios,salas,argumento);
    error = pthread_mutex_unlock(&salas->mutex);
    if (error != 0) {
      errno = error;
      perror("Error intentando salir de la lista de salas unlock: ");
      exit(-1);
    }
    error = pthread_mutex_unlock(&usuarios->mutex);
    if (error != 0) {
      errno = error;
      perror("Error intentando salir de la lista de usuarios ulock: ");
      exit(-1);
    }    
    respuestaAutomatica(fd,"fue");
	}
  else if (strcmp(comando,"men")==0 && strcmp(argumento," ")!=0) {
    error = pthread_mutex_lock(&usuarios->mutex);
    if (error != 0) {
      errno = error;
      perror("Error intentando entrar a la lista de salas lock: ");
      exit(-1);
    }
    error = pthread_mutex_lock(&salas->mutex);
    if (error != 0) {
      errno = error;
      perror("Error intentando entrar a la lista de salas lock: ");
      exit(-1);
    }    
    funcionMen(user,argumento);
    error = pthread_mutex_unlock(&salas->mutex);
    if (error != 0) {
      errno = error;
      perror("Error intentando entrar a la lista de salas lock: ");
      exit(-1);
    }
    error = pthread_mutex_unlock(&usuarios->mutex);
    if (error != 0) {
      errno = error;
      perror("Error intentando entrar a la lista de salas lock: ");
      exit(-1);
    }    
  }
  else{
    respuestaAutomatica(fd,"error");
  }  
  return a;
}	