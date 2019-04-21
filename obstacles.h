#ifndef OBS_H
#define OBS_H

#include "env.h"

class Obstacle {
public:
  int position;
  int opening;

  Obstacle();
  Obstacle(int pos, int op);
  // int get_pos() { return this.position; }
  // int get_op() { return this.opening; }
};

void generate_obstacles(Obstacle** o);    // generate obstacles with random openings
void render_obstacle(Obstacle* obs);    //render obstacles
void update_value(Obstacle** obs);    //update position of obstacles after each display cycle

#endif
