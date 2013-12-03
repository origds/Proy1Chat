/* Archivo: cchat.c
 * Autores: Oriana Gomez   09-10336
 *          Ivan Travecedo 08-11131
 * Descripcion: Manejo del cliente para el chat
 *
 */



#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include "menucchat.h"
#include "manejarArchivo.h"
#include "lista.h"

#define MAXDATASIZE 100  /*Tamano maximo definido para los mensajes */

int socketcliente; /* Entero para el manejo del socket del cliente*/

/* Definimos la estructura mensaje que sera enviada al servidor */
typedef struct Mensaje {
    char *nombreuser;
    char *contenidoMensaje; 
  } Mensaje;


/* Funcion: sizeofMensaje
 * Descripcion: Retorna la suma de los tamanos del nombre
 *              de usuario y el mensaje que este envia
 * Parametro: Mensaje msj
 * Return: Tamano de msj
 */
int sizeofMensaje(Mensaje msj) {
  int size;
  size = strlen(msj.nombreuser) + strlen(msj.contenidoMensaje);
  return size;
}

/* Funcion: conectarSocket
 * Descripcion: obtiene la informacion del servidor, crea el
 *              socket y devuelve el file descriptor para el
 *              manejo del socket
 * Retorna: file descriptor del socket
 */
int conectarSocket(){

  int socketfd;
  struct hostent *infoserver;
  struct sockaddr_in servidor;

  if ((infoserver=gethostbyname(IP))==NULL) {
    printf("gethostbyname() error\n");
    exit(-1);
  }

  if ((socketfd=socket(AF_INET, SOCK_STREAM, 0))==-1){  
    printf("socket() error\n");
    exit(-1);
  }

  servidor.sin_family = AF_INET;
  servidor.sin_port = htons(puerto);
  servidor.sin_addr = *((struct in_addr *)infoserver->h_addr);
  bzero(&(servidor.sin_zero),8);

  if(connect(socketfd, (struct sockaddr *)&servidor, sizeof(struct sockaddr))==-1){ 
    printf("connect() error\n");
    close(socketfd);
    exit(-1);
  }

  return socketfd;
}

/* Procedimiento: enviarPeticion
 * Descripcion: Envia al servidor el nombre del usuario
 *              y el comando que este ejecuta
 * Parametros:
 *            - int fd: Filedescriptor del socket donde se enviara
 *                      la peticion
 *            - Mensaje mensaje: Mensaje a enviar al servidor
 */
void enviarPeticion(int fd, Mensaje mensaje){
  if (send(fd,mensaje.nombreuser,strlen(mensaje.nombreuser),0)==-1){
    printf("No pudo enviarse el nombreuser al servidor\n");
  }  
  if (send(fd,mensaje.contenidoMensaje,strlen(mensaje.contenidoMensaje),0)==-1){
    printf("No pudo enviarse el mensaje al servidor\n");
  }
}


/* Procedimiento: lectorArchivo
 * Descripcion: Lee un archivo y envia cada una de las peticiones
 *              al servidor
 * Parametros:
 *            - FILE * in: Archivo de donde se obtendran las 
 *                         instrucciones
 *            - int bytes: Tamano del archivo
 *            - char * user: Usuario que envia el archivo de 
 *                           peticiones
 *            - int fd: file descriptor del socket donde se enviaran
 *                      las peticiones
 */
void lectorArchivo(FILE * in, int bytes,char * user, int fd) {
  char lineaTemporal[bytes];
  Mensaje msj;
  msj.contenidoMensaje =(char *)malloc(sizeof(char)*bytes);
  msj.nombreuser =(char *)malloc(sizeof(char)*bytes);
  bzero(msj.contenidoMensaje,bytes);
  bzero(msj.nombreuser,bytes);
  strcpy(msj.nombreuser, user);

  while (feof(in)==0){
    fgets(lineaTemporal,bytes,in);

    if (strcmp(lineaTemporal, "fue\n")==0 || strcmp(lineaTemporal, "fue")==0){
      fue = 1;
      strcpy(msj.contenidoMensaje, "fue");
      enviarPeticion(fd, msj);  
      break;
    }
    strcpy(msj.contenidoMensaje,lineaTemporal);
    enviarPeticion(fd, msj);
    bzero(msj.contenidoMensaje, bytes);
    usleep(1000000);
  }
  free(msj.contenidoMensaje);
  free(msj.nombreuser);
  fclose(in);
}


/* Procedimiento: recibirDeServidor
 * Descripcion: Procedimiento que ejecuta el hilo dedicado a
 *              escuchar lo que se recibe del servidor e
 *              imprimir en pantalla
 * Parametros:
 *     - Elemento * elem: Elemento que tiene el fd de donde
 *                        se recibira las respuesta del serv             
 */
void * recibirDeServidor(Elemento * elem){
  char cmd[MAXDATASIZE];
  int numbytes;

  while(1) {
    bzero(cmd, MAXDATASIZE);
    if ((numbytes=recv(elem->fd,cmd,MAXDATASIZE,0)) == -1){  
      printf("Error en recv()\n");
      exit(-1);
    }
    if(strcmp(cmd,"fue")==0){
      printf("\nCierre de sesion satisfactoria\n");
      break;
    }
    else if(strcmp(cmd,"sus")==0){
      printf("\nSuscripcion satisfactoria\n");
    }
    else if(strcmp(cmd,"suserror")==0){
      printf("\nError en suscripcion\n");
    }
    else if(strcmp(cmd,"des")==0){
      printf("\nDesuscripcion satisfactoria\n");
    }
    else if(strcmp(cmd,"deserror")==0){
      printf("\nError en desuscripcion\n");
    }
    else if(strcmp(cmd,"cre")==0){
      printf("\nCreacion de sala satisfactoria\n");
    }
    else if(strcmp(cmd,"creerror")==0){
      printf("\nError creando sala\n");
    }
    else if(strcmp(cmd,"eli")==0){
      printf("\nEliminacion de sala satisfactoria\n");
    }
    else if(strcmp(cmd,"elierror")==0){
      printf("\nError eliminando sala\n");
    }
    else if(strcmp(cmd,"error")==0){
      printf("\nComando invalido\n");
    }
    else if(strcmp(cmd,"elidefecto")==0){
      printf("\nNo puede borrar la sala por defecto\n");
    }    
    else{
      printf("%s\n", cmd);
    }
  }
  close(elem->fd);
  pthread_exit(NULL);
}


/* Funcion Principal Main */

int main(int argc, char *argv[]) {

  Mensaje msjcliente;
  FILE * file;
  char comando[100];
  char resto[100];
  fue = 0;
  pthread_t hiloEscucha;


  //Llamada al menu
  menucchat(argc, argv);

  msjcliente.nombreuser=(char *)malloc(sizeof(char*)*20);
  msjcliente.contenidoMensaje=(char *)malloc(sizeof(char*)*MAXDATASIZE);
  
  bzero(msjcliente.nombreuser,20);
  bzero(msjcliente.contenidoMensaje, MAXDATASIZE);
  bzero(comando,100);

  strcpy(msjcliente.nombreuser,user);

  socketcliente = conectarSocket();
  Elemento * elem;
  elem = (Elemento *)malloc(sizeof(Elemento));
  elem->fd = socketcliente;
  // Creamos un hilo dedicado solo a escuchar lo que manda el servidor e imprimir
  if (pthread_create(&hiloEscucha, NULL, (void *)&recibirDeServidor,(void *)elem)){
    printf("Error creacion hilo\n");
  }
  // Si hay un archivo de comando es leido
  if (archivo!=NULL) {
    int tam = tamanoArchivo(archivo);
    file = abrirArchivo(archivo);
    lectorArchivo(file, tam,msjcliente.nombreuser,socketcliente);
  }
  // si no se encontro fue se pide por consola
  if (fue!=1) {
    while (1) {
      scanf("%s", comando);
      strcpy(msjcliente.contenidoMensaje,comando);
      if (strcmp(comando,"men")==0 || strcmp(comando,"sus")==0 || 
        strcmp(comando,"cre")==0 || strcmp(comando,"eli")==0){
        strcat(msjcliente.contenidoMensaje,gets(resto));
      }
      else if (strcmp(comando, "fue")==0) {
        enviarPeticion(socketcliente, msjcliente);
        break;
      }
      enviarPeticion(socketcliente, msjcliente);
    }
  }
  pthread_join(hiloEscucha,NULL); //esperamos que el hilo que escuche termine
  free(msjcliente.nombreuser);
  free(msjcliente.contenidoMensaje);
  return (0);
}