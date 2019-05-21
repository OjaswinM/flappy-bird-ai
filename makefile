CC=gcc
CFLAGS=-lglut -lGL -std=c++11 -lGLEW -lGLU -lm -g -lstdc++ -lfann -Iincludes/ 

gl: game.cpp 
	$(CC) game.cpp obstacles.cpp bird.cpp  $(CFLAGS)  -o output

train: train.cpp bird.cpp obstacles.cpp genetic.cpp
	$(CC) train.cpp obstacles.cpp bird.cpp genetic.cpp $(CFLAGS) -o train

train-rl: train_rl.cpp rl.cpp 
	$(CC) train_rl.cpp rl.cpp obstacles.cpp bird.cpp $(CFLAGS) -o train_rl

