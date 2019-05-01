CC=gcc
CFLAGS=-lglut -lGL -lGLEW -lGLU -lm -g -lstdc++ -Iincludes/ 

gl: game.cpp 
	$(CC) game.cpp obstacles.cpp bird.cpp  $(CFLAGS)  -o output 
