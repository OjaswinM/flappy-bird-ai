CC=gcc
CFLAGS=-lglut -lGL -lGLEW -lGLU -lstdc++ 

gl: test.cpp 
	$(CC) test.cpp obstacles.cpp  $(CFLAGS)  -o output 
