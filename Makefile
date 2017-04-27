# Makefile Intelligence Artificielle 2016/2017
#
# @author : Chloé Bensoussan
# Université Aix-Marseille

SHELL = /bin/bash
CC    = gcc -g

LIBS = -lm 
CCFLAGS = -Wall -w -ggdb

OBJ = generateurPigeon.o generateurDame.o util.o pile.o

all: csp

csp: csp.c $(OBJ)
	$(CC) $(CCFLAGS) -o csp $^

generateurPigeon.o: generateurPigeon.c
	$(CC) $(CCFLAGS) -c generateurPigeon.c

generateurDame.o: generateurDame.c
	$(CC) $(CCFLAGS) -c generateurDame.c

util.o: util.c
	$(CC) $(CCFLAGS) -c util.c

pile.o: pile.c
	$(CC) $(CCFLAGS) -c pile.c

.PHONY : clean

clean:
	- rm -f $(OBJ)
	- rm -f csp
	- rm -f *.txt
