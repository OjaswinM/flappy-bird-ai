#include "obstacles.h"

#ifndef BIRD_H
#define BIRD_H


class Bird
{
public:
  int x, y;
  float ver_speed;
  int time, last_jump_time;
  float jump_acc;
  Obstacle* next_obs = NULL;

  Bird();
  Bird(ObstacleList ol);
  void render_bird();
  /* update bird's position for the next frame based on gravity and
   * user input */
  void update_pos();
  void jump();
  /* Check if the upcoming obstacle has been crossed */
  bool has_crossed();
};

#endif
