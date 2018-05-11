OBJ=obj/utilidades.o obj/subgradiente.o obj/main.o obj/bb.o
HDR=src/utilidades.h src/subgradiente.h src/estruturas.h src/bb.h
CFLAGS=-g -Wall -c
LFLAGS=-g -L/usr/local/lib

all: main

main: $(OBJ) $(HDR)
	gcc $(LFLAGS) -o main $(OBJ) -lglpk

obj/main.o: src/main.c $(HDR)
	gcc $(CFLAGS) src/main.c -o obj/main.o

obj/utilidades.o: src/utilidades.c $(HDR)
	gcc $(CFLAGS) src/utilidades.c -o obj/utilidades.o

obj/subgradiente.o: src/subgradiente.c $(HDR)
	gcc $(CFLAGS) src/subgradiente.c -o obj/subgradiente.o

obj/bb.o: src/bb.c $(HDR)
	gcc $(CFLAGS) src/bb.c -o obj/bb.o

indutor: aux/indutor.c
	gcc aux/indutor.c -o indutor

completo: aux/completo.c
	gcc aux/completo.c -o completo

clean:
	rm -fr src/*~ obj/* *~ input/*~ main indutor completo aux/*~

