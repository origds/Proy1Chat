/* Archivo: manejarArchivo.c
 * Autores: Oriana Gomez   09-10336
 *          Ivan Travecedo 08-11131
 * Descripcion: Funciones para el manejo de archivo
 */



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include "manejarArchivo.h"

int fue = 0;

/* Retorna el tamano de un archivo */
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

/* abre un archivo */
FILE * abrirArchivo(char *nom){
	FILE *in;

    /*Se abre el archivo nom*/
	in = fopen(nom, "r");
	if (in == NULL){
		printf("Error abriendo el archivo\n");
		return NULL;
	}

	return in;
}