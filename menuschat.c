/* Grupo
	Oriana Gomez	09-10336
	Iván Travecedo	08-11131
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>
#include "menuschat.h"

// menu
	// puerto por defecto: 20336
	// sala por defecto: chatroom

int puerto;
char *sala;

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

int menuschat(int argc, char *argv[]){

	if(argc>5){
		printf("ERROR: La sintaxis de schat es:\nschat [-p <puerto>] [-s <sala>]\n");
		exit(1);
	} else {
		asignarParametros(argv, argc);
	}

	return(0);
}