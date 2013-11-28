/*
 * Archivo Principal de schat. Funciona como servidor del Chat
 * Relaizado por: Oriana Gomez 09-10336
                  Ivan Travecedo 08-11131
 */

#include <stdio.h>          
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "menuschat.h"

#define MAXDATASIZE 100   
#define PORT 20336/* El puerto que será abierto */
#define READ 0
#define WRITE 1

int socketservidor, socketcliente; /* los ficheros descriptores */

void manejoSocket(int enviaPeticionServer[], int enviaRespuestaClient[]) {

   char usr[MAXDATASIZE], cmd[MAXDATASIZE];/* en donde es almacenará el texto recibido */
   struct sockaddr_in servidor; /* para la información de la dirección del servidor */
   struct sockaddr_in cliente; /* para la información de la dirección del cliente */
   int tamsocket, numbytes;
   char bienvenida[] = "Bienvenido a IOChat. Su sala por defecto es: ";

   /*Declaramos lo necesario para crear los procesos hijos*/
   id_t hijo;
   char * bufinst, *buf;
   bufinst = (char*)malloc(sizeof(char)*100);
   strcat(bienvenida, sala);

   if ((socketservidor=socket(AF_INET, SOCK_STREAM, 0)) == -1 ) {  
      printf("error en socket()\n");
      exit(-1);
   }

   servidor.sin_family = AF_INET;         
   servidor.sin_port = htons(PORT);
   servidor.sin_addr.s_addr = INADDR_ANY; /* inet_addr("127.0.0.1") coloca nuestra dirección IP automáticamente */

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

      /*Acepto nueva solicitud de cliente*/

      if ((socketcliente = accept(socketservidor,(struct sockaddr *)&cliente, &tamsocket))==-1) {
         printf("error en accept()\n");
         exit(-1);
      }

      /* Recibo el nombre de usuario del cliente */
      bzero(usr, sizeof(usr));
      if ((numbytes=recv(socketcliente,usr,MAXDATASIZE,0)) == -1){  
         /* llamada a recv() */
         printf("Error en recv() 1 \n");
         exit(-1);
      }
      printf("User: %s\n", usr);
      

      /*Creo al proceso hijo que atendera a ese cliente*/
      if ((hijo = fork())<0){
         perror("fork");
         exit(EXIT_FAILURE);
      }
      
         
      if (hijo==0) {/* Si es hijo*/

         close(enviaPeticionServer[READ]);
         bzero(cmd, sizeof(cmd));

         while(1) {

            if ((numbytes=recv(socketcliente,cmd,MAXDATASIZE,0)) == -1){  
               /* llamada a recv() */
               printf("Error en recv() 2\n");
               exit(-1);
            }

            printf("Inst: %s\n", cmd);

            write(enviaPeticionServer[WRITE], cmd, sizeof(cmd));
            printf("Write pipe: %s\n", cmd);

         }
      }

      if (hijo>0) { /*Si es el padre*/

         while(1) {

            close(enviaPeticionServer[WRITE]);
            bzero(bufinst, sizeof(bufinst));

            read(enviaPeticionServer[READ], bufinst, sizeof(bufinst));
            printf("Read pipe: %s\n", bufinst);

            printf("User: %s\n", usr);

            existeUsuario(usr, sala, socketcliente);
            generarInstrucciones(sala, usr, bufinst);
            /*********************************************************/

            printf("Se obtuvo una conexión desde %d\n",
                   inet_ntoa(cliente.sin_addr) ); 
            /* que mostrará la IP del cliente */

            send(socketcliente,bienvenida,strlen(bienvenida),0);
            /* que enviará el mensaje de bienvenida al cliente */
         }
      }




   }

   printf("CERRANDO SOCKETCLIENTE\n");
   close(socketcliente); /* cierra fd2 */

}

int main(int argc, char *argv[]) {

   int enviaPeticionServer[2], enviaRespuestaClient[2];

   menuschat(argc, argv);

   crearUsuariosSalas(sala);

   pipe(enviaPeticionServer);
   pipe(enviaRespuestaClient);

   manejoSocket(enviaPeticionServer, enviaRespuestaClient);
   close(socketservidor);   

   return(0);
}
