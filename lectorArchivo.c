#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include "lectorArchivo.h"

int fue = 0;

int tamanoArchivo(char * archivo){
	int bytes = 0;
	FILE * in;

	in = fopen(archivo, "r");
	if (in == NULL){
		printf("Error abriendo el archivo\n");
		exit(1);
	}
	
	fseek(in, 0L, SEEK_END);
	bytes = ftell(in);
	
	fclose(in);
	return bytes;	
}

char* lectorArchivo(char *nom, char *contenido){
	int primero = 0;
	FILE *in;
	int bytes = tamanoArchivo(nom);
	contenido = (char *)malloc(sizeof(char*)*bytes);
	char lineaTemporal[bytes];

    /*Se abre el archivo nom*/
	in = fopen(nom, "r");
	if (in == NULL){
		printf("Error abriendo el archivo\n");
		return NULL;
	}
	
	while (feof(in)==0){
		fgets(lineaTemporal,bytes,in);

		if ((strcmp(lineaTemporal, "fue\n")==0) || (strcmp(lineaTemporal, "fue")==0)){
			fue = 1;
			break;
		}

		if (primero == 0){
			strcpy(contenido, lineaTemporal);
			primero++;
		}
		else{
			strcat(contenido, lineaTemporal);
		}
	}
	
  /*Cerramos in*/
	fclose(in);
	return contenido;
}