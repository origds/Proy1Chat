/* Estos son los ficheros de cabecera usuales */
#include <stdio.h>          
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include "menuschat.h"

#define MAXDATASIZE 100   
#define PORT 20336/* El puerto que será abierto */
//#define BACKLOG 2 /* El número de conexiones permitidas */

int socketservidor, socketcliente; /* los ficheros descriptores */

void manejoSocket() {

   char usr[MAXDATASIZE], cmd[MAXDATASIZE];/* en donde es almacenará el texto recibido */
   struct sockaddr_in servidor; /* para la información de la dirección del servidor */
   struct sockaddr_in cliente; /* para la información de la dirección del cliente */
   int tamsocket, numbytes;
   char bienvenida[] = "Bienvenido a IOChat. Su sala por defecto es: ";

   strcat(bienvenida, sala);

   if ((socketservidor=socket(AF_INET, SOCK_STREAM, 0)) == -1 ) {  
      printf("error en socket()\n");
      exit(-1);
   }

   servidor.sin_family = AF_INET;         
   servidor.sin_port = htons(PORT);
   servidor.sin_addr.s_addr = inet_addr("127.0.0.1"); /* INADDR_ANY coloca nuestra dirección IP automáticamente */

   bzero(&(servidor.sin_zero),8); 

   if(bind(socketservidor,(struct sockaddr*)&servidor, sizeof(struct sockaddr))==-1) {
      printf("error en bind() \n");
      exit(-1);
   }     

   if(listen(socketservidor,SOMAXCONN) == -1) {  /* llamada a listen() */
      printf("error en listen()\n");
      exit(-1);
   }

   while(1) {

      /* A continuación la llamada a accept() */
      tamsocket=sizeof(struct sockaddr_in);
      bzero(usr, sizeof(usr));
      bzero(cmd, sizeof(cmd));

      if ((socketcliente = accept(socketservidor,(struct sockaddr *)&cliente, &tamsocket))==-1) {
         printf("error en accept()\n");
         exit(-1);
      }


      if ((numbytes=recv(socketcliente,usr,MAXDATASIZE,0)) == -1){  
         /* llamada a recv() */
         printf("Error en recv() 1 \n");
         exit(-1);
      }


      if ((numbytes=recv(socketcliente,cmd,MAXDATASIZE,0)) == -1){  
         /* llamada a recv() */
         printf("Error en recv() 2\n");
         exit(-1);
      }

      /*** Aqui debe ir la lectura del buffer con las instrucciones ****/
      /*** Y ejecutarlas en los hilos. Creo que tambien va lo de    ****/
      /*** Esperar comandos por consola si no esta 'fue'            ****/
      /*** usar la funcion strtok para separar por lineas (\n)      ****/
      printf("Nombreeeee: %s\n",usr);
      printf("Archivoooo: %s\n",cmd);

      generarInstrucciones(cmd);
      /*********************************************************/

      printf("Se obtuvo una conexión desde %d\n",
             inet_ntoa(cliente.sin_addr) ); 
      /* que mostrará la IP del cliente */

      send(socketcliente,bienvenida,strlen(bienvenida),0);
      /* que enviará el mensaje de bienvenida al cliente */

   }

   close(socketcliente); /* cierra fd2 */

}

int main(int argc, char *argv[])
{

   menuschat(argc, argv);

   manejoSocket();
   close(socketservidor);   

   return(0);
}
