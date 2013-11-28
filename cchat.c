#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include "menucchat.h"
#include "manejarArchivo.h"
/* netbd.h es necesitada por la estructura hostent ;-) */


#define MAXDATASIZE 100   
/* El número máximo de datos en bytes */

int socketcliente;

/* Definimos la estructura mensaje que sera enviada al servidor */

typedef struct Mensaje {
    char *nombreuser;
    char *contenidoMensaje; 
  } Mensaje;

int sizeofMensaje(Mensaje msj) {
  int size;
  size = strlen(msj.nombreuser) + strlen(msj.contenidoMensaje);
  return size;
}

/* Definicion del procedimiento que manejara el socket de cliente */

int conectarSocket(Mensaje mensaje) {

  int socketfd; /* ficheros descriptores */
  struct hostent *infoserver; /* estructura que recibirá información sobre el nodo remoto */
  struct sockaddr_in servidor; /* información sobre la dirección del servidor */

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
  /* htons() es necesaria nuevamente ;-o */
  servidor.sin_addr = *((struct in_addr *)infoserver->h_addr);  
  /*infoserver->h_addr pasa la información de ``*infoserver'' a "h_addr" */
  bzero(&(servidor.sin_zero),8);

  if(connect(socketfd, (struct sockaddr *)&servidor, sizeof(struct sockaddr))==-1){ 
    printf("connect() error\n");
    close(socketfd);
    exit(-1);
  }

  printf("%d y %s\n", strlen(mensaje.nombreuser), mensaje.nombreuser);
  if (send(socketfd,mensaje.nombreuser,sizeof(mensaje.nombreuser),0)==-1){
    printf("No pudo enviarse el nombre de usuario al servidor\n\n");
  }
  
  return socketfd;

}


void enviarPeticion(int fd, Mensaje mensaje) {

  int numbytes;
  char buf[MAXDATASIZE]; /* en donde es almacenará el texto recibido */

  printf("%d y %s \n", strlen(mensaje.contenidoMensaje), mensaje.contenidoMensaje);
  if (send(fd,mensaje.contenidoMensaje,sizeof(mensaje.contenidoMensaje),0)==-1){
    printf("No pudo enviarse el mensaje al servidor\n");
  } 

  printf("FD %d\n", fd);
  if ((numbytes=recv(fd,buf,MAXDATASIZE,0)) == -1){  
    printf("Error en recv() de cliente\n\n");
    exit(-1);
  }

  buf[numbytes]='\0';

  printf("Servidor: %s\n",buf);

}

void lectorArchivo(FILE * in, int bytes, Mensaje msj) {

  char lineaTemporal[bytes];

  while (feof(in)==0){
      fgets(lineaTemporal,bytes,in);

      if ((strcmp(lineaTemporal, "fue\n")==0) || (strcmp(lineaTemporal, "fue")==0)){
        fue = 1;
        bzero(msj.contenidoMensaje, sizeof(msj.contenidoMensaje));
        strcpy(msj.contenidoMensaje, "fue ");
        strcat(msj.contenidoMensaje, user);
        //strcat(msj.contenidoMensaje, "\n");
        printf("CONTENIDO MENSAJE %s\n", msj.contenidoMensaje);
        //socketcliente = conectarSocket(msj);
        enviarPeticion(socketcliente, msj);  
        break;
      }

      strcpy(msj.contenidoMensaje,lineaTemporal);
      //strcat(msj.contenidoMensaje, "\n");
      //socketcliente = conectarSocket(msj);
      enviarPeticion(socketcliente, msj);
  }
    
    /*Cerramos in*/
    fclose(in);

}


/* Funcion Principal Main */

int main(int argc, char *argv[]) {

  Mensaje msjcliente;
  FILE * file;
  char comando[100];
  char resto[100];
  fue = 0;

  //Llamada al menu
  menucchat(argc, argv);

  msjcliente.nombreuser=(char *)malloc(sizeof(char*)*20);
  strcpy(msjcliente.nombreuser,user);
  msjcliente.contenidoMensaje=(char *)malloc(sizeof(char*)*MAXDATASIZE);

  /*Connect al servidor*/

  socketcliente = conectarSocket(msjcliente);
  printf("socketcliente %d\n", socketcliente);

  // Si hay un archivo de comando es leido
  if (archivo!=NULL) {
    int tam = tamanoArchivo(archivo);
    file = abrirArchivo(archivo);
    lectorArchivo(file, tam, msjcliente);
  }

  if (fue!=1) {
    //Para leer comandos por consola
    while (1) {
      printf("Escriba el comando que desea utilizar:\n");
      scanf("%s", comando);
      strcpy(msjcliente.contenidoMensaje,comando);
      if (strcmp(comando,"men")==0 || strcmp(comando,"sus")==0 || strcmp(comando,"cre")==0 || strcmp(comando,"eli")==0){
        strcat(msjcliente.contenidoMensaje,gets(resto));
      }
      //strcat(msjcliente.contenidoMensaje, "\n");
      printf("comando %s\n", msjcliente.contenidoMensaje);
      if (strcmp(msjcliente.contenidoMensaje, "fue")==0) {
        strcat(msjcliente.contenidoMensaje, " ");
        strcat(msjcliente.contenidoMensaje, msjcliente.nombreuser);
        //strcat(msjcliente.contenidoMensaje, "\n");
        //socketcliente = conectarSocket(msjcliente);
        enviarPeticion(socketcliente, msjcliente);
        close(socketcliente);
        return(0);
      } else {
        //socketcliente = conectarSocket(msjcliente);
        enviarPeticion(socketcliente, msjcliente);
        continue;
      }
    }
   
  }
  
  free(msjcliente.nombreuser);
  free(msjcliente.contenidoMensaje);
  return (0);

}