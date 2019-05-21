#include <iostream>
#include <fstream>
#include "genetic.h"
#include "obstacles.h"
#include "env.h"


int main()
{
  ObstacleList ol;
  int layers[] = {2, 6, 1};
  ol.generate_obstacles();

  GA ga(population_size, 5, ol, 3, layers);

  ga.init_rand_pop();

  for (int i=0; i<200; i++) {

    if(ga.population[0]->score > 8) {
      fann_save(ga.population[0]->ann, "good_network");
    }

    std::cout << "Iteration " << i << " -----------------\n";
    ga.simulate_game_gen();
    ga.evolve();
  }

  std::ofstream myfile;

  std::cout << "Saving network" << std::endl;

  bool success = fann_save(ga.population[0]->ann, "trained_network");
  if (success == 0) {
    std::cout << "Success";
  } else {
    std::cout << "Failed to save file";
  }
}
