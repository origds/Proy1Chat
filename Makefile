# Makefile para cchat y schat
# Autores Oriana Gomez		09-10336
#	 	   Ivan Travecedo	08-11131
# hay que agregar -pthread cuando se trabajen los hilos!
all: cchat schat
cchat: cchat.o menucchat.o manejarArchivo.o
	gcc menucchat.o manejarArchivo.o cchat.o -o cchat
cchat.o: cchat.c menucchat.h manejarArchivo.h
	gcc -c cchat.c
menucchat.o: menucchat.c menucchat.h
	gcc -c menucchat.c
manejarArchivo.o: manejarArchivo.c manejarArchivo.h
	gcc -c manejarArchivo.c
schat: schat.o menuschat.o procesarinstrucciones.o lista.o
	gcc menuschat.o schat.o procesarinstrucciones.o lista.o -o schat
schat.o: schat.c menuschat.h
	gcc -c schat.c
menuschat.o: menuschat.c menuschat.h
	gcc -c menuschat.c
procesarinstrucciones.o: procesarinstrucciones.c procesarinstrucciones.h lista.h
	gcc -c procesarinstrucciones.c
lista.o: lista.c lista.h
	gcc -c lista.c
clean:
	rm *.o cchat schat