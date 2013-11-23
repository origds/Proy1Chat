#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include "procesarinstrucciones.h"


void generarInstrucciones(char * instruccion) {

  char * token;

  token = strtok (instruccion,"\n");
  token = strtok(token," ");
  if(token!=NULL && (strcmp(token,"men")==0 || strcmp(token,"sus")==0 || strcmp(token,"cre")==0 || strcmp(token,"eli")==0)){
     printf("Comando:%s\n", token);
     token = strtok (NULL,"\n");
     printf("Argumento:%s\n", token);
  } else {
     printf("Comando:%s\n", token);
  }  
}

void procesarInstrucciones(char * comando, char * argumento) {


}

