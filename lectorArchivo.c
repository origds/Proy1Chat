#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

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

int lectorPalabra(char *nom){
	int i,count = 0;
	FILE *in;
	char buff[tamanoArchivo(nom)];

    /*Se abre el archivo nom*/
	in = fopen(nom, "r");
	if (in == NULL)
	{
		printf("Error abriendo el archivo\n");
		exit(1);
	}
	
	while (feof(in)==0)
	{
		fscanf(in,"%s",buff);
		printf("En el buffer= %s\n", buff);
		
	}
	
    /*Cerramos in*/
	fclose(in);
	return (count);	
}

int main(int argc, char *argv[]){
	//printf("tamano archivo: %d\n", tamanoArchivo(argv[1]));
	lectorPalabra(argv[1]);
	return 0;
}