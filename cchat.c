#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include "menucchat.h"
#include "lectorArchivo.h"
/* netbd.h es necesitada por la estructura hostent ;-) */


#define MAXDATASIZE 100   
/* El número máximo de datos en bytes */


int main(int argc, char *argv[])
{
   int fd, numbytes;       
   /* ficheros descriptores */

   char buf[MAXDATASIZE];
   /* en donde es almacenará el texto recibido */

   struct hostent *infoserver;         
   /* estructura que recibirá información sobre el nodo remoto */

   struct sockaddr_in servidor;  
   /* información sobre la dirección del servidor */

   char * contenidoArchivo;

   //llamada al menu

   menucchat(argc, argv);

   contenidoArchivo = lectorArchivo(archivo, contenidoArchivo);
   //printf("Contenido archivooo: %s\n", contenidoArchivo);

   if ((infoserver=gethostbyname(IP))==NULL) {
      /* llamada a gethostbyname() */
      printf("gethostbyname() error\n");
      exit(-1);
   }

   if ((fd=socket(AF_INET, SOCK_STREAM, 0))==-1){  
      /* llamada a socket() */
      printf("socket() error\n");
      exit(-1);
   }

   servidor.sin_family = AF_INET;
   printf("puertoooo: %d",puerto);
   servidor.sin_port = htons(puerto);
   /* htons() es necesaria nuevamente ;-o */
   servidor.sin_addr = *((struct in_addr *)infoserver->h_addr);  
   /*infoserver->h_addr pasa la información de ``*infoserver'' a "h_addr" */
   bzero(&(servidor.sin_zero),8);

   if(connect(fd, (struct sockaddr *)&servidor,
      sizeof(struct sockaddr))==-1){ 
      /* llamada a connect() */
      printf("connect() error\n");
      exit(-1);
   }

   send(fd,contenidoArchivo,strlen(contenidoArchivo),0); 
      /* mensaje con el contenido del archivo al servidor */

   if ((numbytes=recv(fd,buf,MAXDATASIZE,0)) == -1){  
      /* llamada a recv() */
      printf("Error en recv() \n");
      exit(-1);
   }

   buf[numbytes]='\0';

   printf("Mensaje del Servidor: %s\n",buf); 
   /* muestra el mensaje de bienvenida del servidor =) */

   close(fd);   /* cerramos fd =) */

}