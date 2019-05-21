#ifndef OBS_H
#define OBS_H

class Obstacle {
public:
  int init_position;
  int position;
  int opening;
  Obstacle *next_obs;
  Obstacle *prev_obs;

  Obstacle();
  Obstacle(int pos, int op);
  void render_obstacle();    //render obstacles
  void update_pos();
  void reset();
};

class ObstacleList {
public:
  Obstacle* obs_list_head;
  Obstacle* obs_list_rear;

  ObstacleList();
  // ObstacleList(int size);
  void add_rear(Obstacle *obs);
  void render_obstacles();
  void generate_obstacles();    // generate obstacles with random openings
  void update_pos();    //update position of obstacles after each display cycle
  void display_list();
  void reset_list();
};


#endif
