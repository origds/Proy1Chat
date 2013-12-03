/* Archivo: lista.c
 * Autores: Oriana Gomez   09-10336
 *          Ivan Travecedo 08-11131
 * Descripcion: Implementacion de funciones para el manejo del
 *              menu del servidor (para su arranque)
 */


#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>
#include "menuschat.h"
#include "lista.h"

// variables globales para puerto y sala
int puerto;
char *sala;

/* Inicializa las variables globales recibidas desde consola
 * en caso de error imprime un mensaje para el usuario
 */
void asignarParametros(char *param[], int tam) {

	int p = 0;
	int s = 0;
	int i = 0;
	sala = (char *)malloc(sizeof(char)*20);

	for ( i = 1 ; i < tam-1 ; i++ ) {

		if (strcmp(param[i],"-p")==0 && !p){
			puerto = atoi(param[i+1]);
			p = 1;
		}
		else if (strcmp(param[i],"-s")==0 && !s){
			sala = param[i+1];
			s = 1;
		}
		else{ 
			printf("ERROR: La sintaxis de schat es:\nschat [-p <puerto>] [-s <sala>]\n");
			exit(1);
		}

		i = i+1;
	}

	if (!p) {
		puerto = 20336;
		p = 1;
	} else if (!s) {
		sala = "chatroom";
		s = 1;
	}
	

}

// Toma los datos de consola y llama a asignarParametros
int menuschat(int argc, char *argv[]){

	if(argc>5){
		printf("ERROR: La sintaxis de schat es:\nschat [-p <puerto>] [-s <sala>]\n");
		exit(1);
	} else {
		asignarParametros(argv, argc);
	}

	return(0);
}