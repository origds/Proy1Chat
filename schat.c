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

main()
{

   char buf[MAXDATASIZE];
   /* en donde es almacenará el texto recibido */

   int fdservidor, fdcliente; /* los ficheros descriptores */

   struct sockaddr_in servidor; 
   /* para la información de la dirección del servidor */

   struct sockaddr_in cliente; 
   /* para la información de la dirección del cliente */

   int tamsocket, numbytes;

   //menuschat(argc, argv);

   /* A continuación la llamada a socket() */
   if ((fdservidor=socket(AF_INET, SOCK_STREAM, 0)) == -1 ) {  
      printf("error en socket()\n");
      exit(-1);
   }

   servidor.sin_family = AF_INET;         

   servidor.sin_port = htons(PORT); 
   /* ¿Recuerdas a htons() de la sección "Conversiones"? =) */

   servidor.sin_addr.s_addr = inet_addr("127.0.0.1"); 
   /* INADDR_ANY coloca nuestra dirección IP automáticamente */

   bzero(&(servidor.sin_zero),8); 
   /* escribimos ceros en el reto de la estructura */


   /* A continuación la llamada a bind() */
   if(bind(fdservidor,(struct sockaddr*)&servidor, sizeof(struct sockaddr))==-1) {
      printf("error en bind() \n");
      exit(-1);
   }     

   if(listen(fdservidor,SOMAXCONN) == -1) {  /* llamada a listen() */
      printf("error en listen()\n");
      exit(-1);
   }

   while(1) {
      tamsocket=sizeof(struct sockaddr_in);
      /* A continuación la llamada a accept() */
      if ((fdcliente = accept(fdservidor,(struct sockaddr *)&cliente,
                        &tamsocket))==-1) {
         printf("error en accept()\n");
         exit(-1);
      }

      /*********************************************************/
      /*** Lo agregue para recibir los parametros del cliente***/
      if ((numbytes=recv(fdcliente,buf,MAXDATASIZE,0)) == -1){  
         /* llamada a recv() */
         printf("Error en recv() \n");
         exit(-1);
      }

      /*** Aqui debe ir la lectura del buffer con las instrucciones ****/
      /*** Y ejecutarlas en los hilos. Creo que tambien va lo de    ****/
      /*** Esperar comandos por consola si no esta 'fue'            ****/
      /*** usar la funcion strtok para separar por lineas (\n)      ****/
      printf("Archivoooo:\n%s\n",buf);
      /*********************************************************/

      printf("Se obtuvo una conexión desde %d\n",
             inet_ntoa(cliente.sin_addr) ); 
      /* que mostrará la IP del cliente */

      send(fdcliente,"Bienvenido a mi servidor.\n",27,0); 
      /* que enviará el mensaje de bienvenida al cliente */

      close(fdcliente); /* cierra fd2 */
   }

   return(0);
}
