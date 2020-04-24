# Flappy Bird AI: Game Playing with Genetic Algorithm

This was a project me and my project partner developed for our 3rd year Computer Graphics course. This project
is based on OpenGL and is completely written in C++. I decided using C++ over
Javascript, the language I had most experience in at that time, for multiple reasons.
Firstly, OpenGL is itself written and C/C++ and thus has a stable, well written implementation
for C++ and secondly, after working on a high level language like Javascript, I wanted to try
out something different to gain a new perpective and C++ seemed like a good option.

In my 2nd year, I had worked on a basic demonstration of [Genetic Algortihm](https://en.wikipedia.org/wiki/Genetic_algorithm)
and thought it would be a good idea to extend the project. Me and my project partner decided
to create a [Flappy Bird](https://flappybird.io/) like game with a focus on **using
genetic algorithm to train the computer** to play the game.

## Overview

The development of this project was divided into 2 major phases:-

1. **Creating the Game Using OpenGL**

    The game consists of a simple rectangular bird-like object which only has the
    ability to jump. The aim is to time the jumps so as to cross the randomly generated
    obstacles without colliding. The user is scored based on the number of obstacles crossed,
    much like the original Flappy Bird game.

2. **Implementing the ANN and Genetic Algorithm**

    The second phase involved laying out the code to enable the computer to
    understand and learn to play the game. This was done by attaching the bird with an
    [*Artificial Neural Network(ANN)*](https://en.wikipedia.org/wiki/Artificial_neural_network)
    that determined when to jump. Now, the challenge here was to train the ANN to achieve desired results.
    This training was done through [genetic algorithm(GA)](https://en.wikipedia.org/wiki/Genetic_algorithm)
    which was used to tweak the hyper paramaters of the network in order to maximise the final score.
    
 ---   
*You can know more about the implementation and source code of the project [here](https://ojaswinm.github.io/post/fourth/)*
