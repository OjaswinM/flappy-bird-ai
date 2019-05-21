#include "bird.h"
#include "obstacles.h"

#ifndef GEN_H
#define GEN_H

class GA {
public:
  int pop_size, num_winners, iteration, best_score, num_layers, *layers, mutation_rate;
  NeuralBird **population, **last_population;
  ObstacleList ol;

  GA(int pop_size, int num_winners, ObstacleList ol, int num_layers, int *layers);
  void init_rand_pop();
  int simulate_game(NeuralBird *nb, ObstacleList ol);
  void simulate_game_gen();
  void display_fitness(NeuralBird** population);
  void crossover(NeuralBird *nb1, NeuralBird *nb2, NeuralBird **child);
  void evolve();
};
bool comp_fitness(NeuralBird *nb1, NeuralBird *nb2);
float rand_float(float a, float b);
#endif
