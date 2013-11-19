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

/* Definimos la estructura mensaje que sera enviada al servidor */

typedef struct Mensaje {
    char *nombreuser;
    char *contenidoMensaje; 
  } Mensaje;

/* Definicion del procedimiento que manejara el socket de cliente */

void manejoSockets(char *mensaje) {

  int fd, numbytes;       
  /* ficheros descriptores */

  char buf[MAXDATASIZE];
  /* en donde es almacenará el texto recibido */

  struct hostent *infoserver;         
  /* estructura que recibirá información sobre el nodo remoto */

  struct sockaddr_in servidor;  
  /* información sobre la dirección del servidor */

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

  send(fd,mensaje,strlen(mensaje),0); 
    /* envia mensaje con el contenido del archivo o el comando al servidor */

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



int main(int argc, char *argv[]) {

  Mensaje msjcliente;

  char* comando; //SUSTITUIR COMANDO POR CONTENIDOMENSAJE CUANDO 
            //DEFINAMOS BIEN QUE HACER CON NUESTRAS VIDAS

  //Llamada al menu
  menucchat(argc, argv);

  // Si hay un archivo de comando es leido
  if (archivo!=NULL) {
      msjcliente.contenidoMensaje = lectorArchivo(archivo, msjcliente.contenidoMensaje);
      printf("Contenido archivooo: %s\n", msjcliente.contenidoMensaje);
      //llamada al procedimiento que creara el socket
      //manejoSockets();
      //llamada al procedimiento que enviara el archivo con el mensaje
      // del archivo     
      manejoSockets(msjcliente.contenidoMensaje);
  }

  //FALTA VERIFICAR QUE SI LA ULTIMA LINEA DEL ARCHIVO ES fue ENTONCES NO
  // DEBE PEDIR COMANDOS POR CONSOLA
   

  //Para leer comandos por consola
  while (1) {
    printf("Escriba el comando que desea utilizar:\n");
    scanf("%s", &comando);
    printf("comando %s\n", comando);
    if (strcmp(comando, "fue")==0) {
      return(0);
    } else {
      //PREGUNTA!! CAPAZ MANEJOSOCKET HAY QUE SEPARARLO EN DOS PORQUE EN 
      //MANEJO SOCKET PODEMOS DEJAR SOLO LA PARTE DE CONEXION Y LUEGO HACEMOS
      //UN PROCEDIMIENTO ENVIAR QUE LO UNICO QUE HAGA SEAN SEND AL SERVER USANDO
      //EL SOCKET QUE YA CREAMOS.

      //llamar aL procedimiento XS para pasarle el comando a traves del socket
      //manejoSockets(comando);
      continue;
    }
  }
   
  manejoSockets(msjcliente.contenidoMensaje);
  
  return (0);

}