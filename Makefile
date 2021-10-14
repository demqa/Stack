# CC = Your compiler
CC=g++

# CFLAGS = CompilatorFLAGS
CFLAGS=-c

all: program

program: main.o stack.o 
	$(CC) main.o stack.o -o program

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp

stack.o: stack.cpp
	$(CC) $(CFLAGS) stack.cpp

clean:
	rm -rf *.o program
