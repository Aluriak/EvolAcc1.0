#Nom du projet
FINALNAME=EvolAcc1.0

#Compilateur
CC=g++

#Flags à mettre
CCFLAGS=-Wall -Werror
CCLIBS=-lSDL

all:
	$(CC) $(CCFLAGS) -c *.cpp
	$(CC) -o $(FINALNAME) *.o $(CCLIBS) 
	rm *.o
