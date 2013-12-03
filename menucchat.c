/* Archivo: lista.c
 * Autores: Oriana Gomez   09-10336
 *          Ivan Travecedo 08-11131
 * Descripcion: Implementacion de funciones para el manejo 
 *              del menu del cliente
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "menucchat.h"

/* variables donde se guardara lo recibido 
 * por linea de comandos
 */
char *IP;
int puerto;
char *user;
char *archivo;

/* Inicializa las variables globales recibidas desde consola
 * en caso de error imprime un mensaje para el usuario
 */
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
    } else if (strcmp(param[i],"-p")==0 && !p) {
        puerto = atoi(param[i+1]);
        p = 1;
    } else if (strcmp(param[i],"-n")==0 && !n) {
        user = param[i+1];
        n = 1;
    } else if (strcmp(param[i],"-a")==0 && !a) {
        archivo = param[i+1];
        a = 1;
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
  } else if (!n) {
    user = "guest";
    n = 1;
  }

  return (falto);
}

// Toma los datos de consola y llama a asignarParametros
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