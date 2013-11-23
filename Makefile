# Makefile para cchat y schat
# Autores Oriana Gomez		09-10336
#	 	   Iván Travecedo	08-11131
# hay que agregar -pthread cuando se trabajen los hilos!
all: cchat schat
cchat: cchat.o menucchat.o lectorArchivo.o
	gcc menucchat.o lectorArchivo.o cchat.o -o cchat
cchat.o: cchat.c menucchat.h lectorArchivo.h
	gcc -c cchat.c
menucchat.o: menucchat.c menucchat.h
	gcc -c menucchat.c
lectorArchivo.o: lectorArchivo.c lectorArchivo.h
	gcc -c lectorArchivo.c
schat: schat.o menuschat.o
	gcc menuschat.o schat.o -o schat
schat.o: schat.c menuschat.h
	gcc -c schat.c
menuschat.o: menuschat.c menuschat.h
	gcc -c menuschat.c
clean:
	rm *.o cchat schat