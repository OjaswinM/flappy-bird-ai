#include "genetic.h"
#include "bird.h"
#include "obstacles.h"
#include "env.h"
#include <iostream>
#include <algorithm>
#include <iterator>
#include <time.h>

GA::GA(int pop_size, int num_winners, ObstacleList ol, int num_layers, int* layers)
{
  this->pop_size = pop_size;
  this->num_winners = num_winners;
  this->num_layers = num_layers;
  this->layers = layers;
  this->population = new NeuralBird*[pop_size];
  this->mutation_rate = mut_rate;
  this->ol = ol;
  this->iteration = 0;
}

void GA::init_rand_pop()
{
  for (int i = 0; i < this->pop_size; i++) {
    this->population[i] = new NeuralBird(this->ol, this->num_layers, this->layers);
  }
}

int GA::simulate_game(NeuralBird *nb, ObstacleList ol)
{
  ol.reset_list();
  nb->reset(ol);

  while (!nb->game_over) {
    nb->Bird::update_pos();
    ol.update_pos();

    if (nb->should_jump()) {
      nb->Bird::jump();
    }
  }

  // nb->avg_fitness = (this->iteration * nb->avg_fitness + nb->score)/(this->iteration + 1);

}

void GA::simulate_game_gen()
{
  for (int i = 0; i < pop_size; i++) {
    this->simulate_game(population[i], this->ol);
  }

  this->iteration++;
  std::sort(this->population, this->population + pop_size, comp_fitness);
}

void GA::display_fitness(NeuralBird** population)
{
  std::cout << "Score\tFitness\tAverage Fitness\tTotal Distance" << std::endl;
  for(int i=0; i < this->pop_size; i++) {
    std::cout << population[i]->score << "\t" << population[i]->distance << "\t" << population[i]->avg_fitness << "\t" << population[i]->total_fitness << std::endl;
  }
}

void GA::crossover(NeuralBird *nb1, NeuralBird *nb2, NeuralBird **child)
{
  struct fann* ann1 = fann_copy(nb1->ann);
  struct fann* ann2 = fann_copy(nb2->ann);
  struct fann* child_ann = fann_copy(ann1);
  *child = new NeuralBird(*nb1);
  (*child)->turns = 0;
  (*child)->total_fitness = 0;
  (*child)->avg_fitness = 0;

  int total_conn = fann_get_total_connections(ann1);
  struct fann_connection *connection1 = new struct fann_connection[total_conn];
  struct fann_connection *connection2 = new struct fann_connection[total_conn];
  struct fann_connection *child_conn = new struct fann_connection[total_conn];

  srand(time(0));

  fann_get_connection_array(ann1, connection1);
  fann_get_connection_array(ann2, connection2);
  fann_get_connection_array(ann1, child_conn);

  int crossover_pt = rand() % total_conn;

  for (int i=0; i<total_conn; i++) {
    // Random mutation
    // if ((rand() % 100) < mutation_rate) {
    //   float rand_weight = rand_float(-1.0, 1.0);
    //   child_conn[i].weight = rand_weight;
    // } else if (i<crossover_pt) {
    //   child_conn[i].weight	= connection1[i].weight;
    // } else {
    //   child_conn[i].weight = connection2[i].weight;
    // }

    if ((rand() % 100) < mutation_rate) {
      float rand_weight = rand_float(-1.0, 1.0);
      child_conn[i].weight = rand_weight;
    } else if ( (connection1[i].from_neuron = 2 || connection1[i].from_neuron == 9) && i > crossover_pt ) {
      child_conn[i].weight = connection2[i].weight;
    } else {
      child_conn[i].weight = connection1[i].weight;
    }

    fann_set_weight_array(child_ann, child_conn, total_conn);
    (*child)->ann = fann_copy(child_ann);
  }
}

void GA::evolve()
{
  NeuralBird *parent1, *parent2;
  this->last_population = new NeuralBird*[this->pop_size];
  // std::copy(this->population, this->population + pop_size, this->last_population);

  for(int i=0; i<this->pop_size; i++)
  {
    this->last_population[i] = new NeuralBird(*(this->population[i]));
  }

  int index=0, curr_index=0;
  // copy 4 best winners to new generation
  for(int i=index; i<0.4*this->pop_size; i++)
  {
    this->population[i] = new NeuralBird(*(this->last_population[i]));
    index++;
  }

  curr_index = index;
  // 1 offspring from 2 best winners
  for(int i=index; i<curr_index + 0.1*this->pop_size; i++)
  {
    parent1 = this->last_population[0];
    parent2 = this->last_population[1];
    this->crossover(parent1, parent2, &this->population[i]);
    index++;
  }

  curr_index = index;
  // 3 offsprings from 2 random winners
  for(int i=index; i<curr_index + 0.3*this->pop_size; i++)
  {
    int index1 = rand() % num_winners;
    int index2 = rand() % num_winners;
    parent1 = this->last_population[index1];
    parent2 = this->last_population[index2];

    this->crossover(parent1, parent2, &this->population[i]);
    index++;
  }

  // 2 direct copies of random winners
  for(int i=index; i<(this->pop_size); i++)
  {
    int winner = rand() % num_winners;

    this->population[i] = this->last_population[winner];
  }

  // for(int i=4; i<pop_size; i++)
  // {
  //   int index1 = rand() % num_winners;
  //   int index2 = rand() % num_winners;
  //   parent1 = this->last_population[index1];
  //   parent2 = this->last_population[index2];
  //
  //   this->population[i] = new NeuralBird(*(this->last_population[i]));
  //   this->crossover(parent1, parent2, this->population[i]);
  // }

  this->simulate_game_gen();
  display_fitness(this->population);
}

bool comp_fitness(NeuralBird *nb1, NeuralBird *nb2)
{
  return nb1->score > nb2->score;
}

float rand_float(float a, float b) {
    float random = ((float) rand()) / (float) RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}
