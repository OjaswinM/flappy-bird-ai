#include "obstacles.h"
#include "fann.h"

#ifndef BIRD_H
#define BIRD_H

struct action_value
{
  float no_jump;
  float jump;
};

class Bird
{
public:
  int x, y, time, last_jump_time, score, distance, total_fitness, turns;
  float ver_speed, jump_acc;
  bool game_over;
  float avg_fitness;
  Obstacle* next_obs = NULL;

  Bird(ObstacleList ol);
  Bird(Bird& b);
  void render_bird();
  /* update bird's position for the next frame based on gravity and
   * user input */
  void update_pos(Obstacle* o = NULL);
  void jump();
  /* Check if the upcoming obstacle has been crossed */
  bool has_crossed(Obstacle* o = NULL);
  bool has_collided(Obstacle* o = NULL);
  void reset(ObstacleList ol);
};

class NeuralBird : public Bird
{
public:
  struct fann *ann;
  fann_type* jump_decision;
	int num_layers;
	int* layers;

  NeuralBird(ObstacleList ol, int num_layers, int* layers);
  NeuralBird(NeuralBird& nb);
  bool should_jump(fann_type* input = NULL);
  struct action_value calc_q(fann_type* input);
  fann_type* get_dist_from_obs();
};

#endif
