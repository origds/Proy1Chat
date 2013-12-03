/* Archivo: procesarinstrucciones.c
 * Autores: Oriana Gomez   09-10336
 *          Ivan Travecedo 08-11131
 * Descripcion: Archivo Principal de schat. Funciona como servidor del Chat
 */

#include <stdio.h>          
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/select.h>
#include "menuschat.h"
#include "lista.h"

#define MAXDATASIZE 100   
#define PORT 20336/* El puerto que será abierto por defecto */

int socketservidor, socketcliente; /* los ficheros descriptores */
ListaThread * listaHilos; // lista de hilos dinamica


/* Procedimiento: printListaPPal
 * Descripcion: procedimiento que ejecuta cada hilo de cada usuario
 *              imprime lo que recibe del servidor
 * Parametro: Thread * elem: elemento que tiene el fd del que va a
 *                            recibir
 */
void * atenderPeticion(Thread * elem){
  char cmd[MAXDATASIZE];/* en donde es almacenara el texto recibido */
  char usr[20];/* en donde es almacenara el usuario recibido */
  int numbytes;
  char bienvenida[] = "Bienvenido a IOChat\n";
  int existe = -1;

  if (send(elem->fd,bienvenida,strlen(bienvenida),0)==-1){
    printf("Error atendiendo peticion\n");
    pthread_exit(NULL);
  }

  while(1){
    bzero(cmd, MAXDATASIZE);
    bzero(usr, 20);
    if ((numbytes=recv(elem->fd,usr,20,0)) == -1){
      printf("Error en recv usuario()\n");
      exit(-1);
    }
    if ((numbytes=recv(elem->fd,cmd,MAXDATASIZE,0)) == -1){
      printf("Error en recv Instruccion()\n");
      exit(-1);
    }
    printf("Usuario recibido: %s\n", usr);
    printf("Instruc recibida: %s\n", cmd);
    if(existe == -1){
      existeUsuario(usr, sala, elem->fd);
      existe = 0;
    }
    generarInstrucciones(sala, usr, cmd, elem->fd);
    if(strcmp(cmd,"fue")==0){
      printf("Usuario %s cerro sesion\n",usr);
      break;
    }
  }
  close(elem->fd);
  pthread_exit(NULL);
}

/* Procedimiento: manejoSocket
 * Descripcion: Se crea el socket del servidor, se asocia el puerto
 *              y se pone a escuchar. Por cada usuario conectado crea
 *              un hilo que lo atiende y sigue esperando nuevas
 *              conexiones
 */
void manejoSocket() {

   char usr[MAXDATASIZE];
   struct sockaddr_in servidor;
   struct sockaddr_in cliente;
   int tamsocket, numbytes;

   listaHilos = (ListaThread *)malloc(sizeof(ListaThread));
   nuevaListaT(listaHilos);

   if ((socketservidor=socket(AF_INET, SOCK_STREAM, 0)) == -1 ) {  
      printf("error en socket()\n");
      exit(-1);
   }

   servidor.sin_family = AF_INET;
   servidor.sin_port = htons(PORT);
   servidor.sin_addr.s_addr = INADDR_ANY;

   bzero(&(servidor.sin_zero),8); 

   if(bind(socketservidor,(struct sockaddr*)&servidor, sizeof(struct sockaddr))==-1) {
      printf("error en bind() \n");
      exit(-1);
   }

   if(listen(socketservidor,SOMAXCONN) == -1){
      printf("error en listen()\n");
      exit(-1);
   }

   /* A continuación la llamada a accept() */
   tamsocket=sizeof(struct sockaddr_in);
   while(1) {
      printf("Esperando por cliente nuevo\n");
      /*Acepto nueva solicitud de cliente*/
      if ((socketcliente = accept(socketservidor,(struct sockaddr *)&cliente, &tamsocket))==-1) {
         printf("error en accept()\n");
         exit(-1);
      }
      printf("socketcliente antes de crear hilo: %d\n", socketcliente);
      // inserto un nuevo elemento en la lista de hilos y tomo el primero
      insertarT(listaHilos,(pthread_t)NULL,socketcliente); 

      if (pthread_create(&(listaHilos->ini->id), NULL, (void *)&atenderPeticion,(void *)(listaHilos->ini))!=0){
        printf("Error creacion hilo");
      }
   }
   close(socketcliente);
}

int main(int argc, char *argv[]) {

   menuschat(argc, argv);
   crearUsuariosSalas(sala);
   manejoSocket();
   close(socketservidor);
   return(0);
}
