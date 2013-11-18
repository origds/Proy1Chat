#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "menucchat.h"

char *IP;
int puerto;
char *user;
char *archivo;

char asignarParametros(char *param[], int tam) {
  int i;
  int h,p,n,a;
  char falto;
  h = 0;
  p = 0;
  n = 0;
  a = 0;

  for ( i = 1 ; i < tam-1 ; i++ ) {

    if (strcmp(param[i],"-h")==0 && !h) {
        IP = param[i+1];
        h = 1;
        printf("IP %s \n",IP);
    } else if (strcmp(param[i],"-p")==0 && !p) {
        puerto = atoi(param[i+1]);
        p = 1;
        printf("puerto %d \n", puerto);
    } else if (strcmp(param[i],"-n")==0 && !n) {
        user = param[i+1];
        n = 1;
        printf("nombre usuario %s \n",user);
    } else if (strcmp(param[i],"-a")==0 && !a) {
        archivo = param[i+1];
        a = 1;
        printf("nombre archivo %s \n", archivo);
    } else {
        printf("ERROR: La sintaxis de cchat es:\n cchat [-h host] [-p puerto] [-n nombre] [-a archivo]\n");
        exit(1); 
    }

    i = i+1;
  }

  if (!h)  {
    falto = 'h';
  } else if (!p) {
    puerto = 20336;
    p = 1;
    //printf("puerto %s \n", puerto);
  } else if (!n) {
    user = "guest";
    n = 1;
    printf("nombre usuario %s \n",user);
  }

  return (falto);
}

void menucchat(int argc, char *argv[])
{
  if (argc>9) {
    printf("ERROR: La sintaxis de cchat es:\n cchat [-h host] [-p puerto] [-n nombre] [-a archivo]\n"); 
    exit(1);
  } else {
      if (argc<9) {
        char paramfalto;
        paramfalto = asignarParametros(argv, argc);
        if (paramfalto == 'h') {
          printf("ERROR: Debe introducir un host obligatorio.\nLa sintaxis de cchat es:\n cchat [-h host] [-p puerto] [-n nombre] [-a archivo]\n");
          exit(1);  
        }
      } else {
        char param;
        param = asignarParametros(argv, argc);
      }
  }

}