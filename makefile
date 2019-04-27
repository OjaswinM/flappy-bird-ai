CC=gcc
CFLAGS=-lglut -lGL -lGLEW -lGLU -lm -g -lstdc++ -Iincludes/ 

gl: test.cpp 
	$(CC) test.cpp obstacles.cpp bird.cpp  $(CFLAGS)  -o output 
