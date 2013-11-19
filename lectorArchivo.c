#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include "lectorArchivo.h"

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

/*int main(int argc, char *argv[]){
	char * contenido;
	printf("En el buffer =\n %s\n", lectorArchivo(argv[1],contenido));
	return 0;
}*/