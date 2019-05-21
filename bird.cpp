#include <GL/glut.h>
#include <math.h>
#include "env.h"
#include "bird.h"
#include "fann.h"

Bird::Bird(ObstacleList ol)
{
  this->x = bird_x;
  this->y = bird_y;
  this->score = 0;
  this->game_over = false;
  this->ver_speed = 0;
  this->time = 0;
  this->jump_acc = 0;
  this->last_jump_time = 0;
  this->distance = 0;
  this->next_obs = ol.obs_list_head;
  this->avg_fitness = 0;
  this->total_fitness = 0;
  this->turns = 0;
}

Bird::Bird(Bird& b)
{
  this->x = b.x;
  this->y = b.y;
  this->score = b.score;
  this->game_over = b.game_over;
  this->ver_speed = b.ver_speed;
  this->time = b.time;
  this->jump_acc = b.jump_acc;
  this->last_jump_time = b.last_jump_time;
  this->distance = b.distance;
  this->next_obs = b.next_obs;
  this->avg_fitness = b.avg_fitness;
  this->total_fitness = b.total_fitness;
  this->turns = b.turns;
}

void Bird::render_bird()
{
  glColor3f(1, 0, 0);

  glBegin(GL_QUADS);
  glVertex2f(this->x - bird_size/2, this->y - bird_size/2);
  glVertex2f(this->x + bird_size/2, this->y - bird_size/2);
  glVertex2f(this->x + bird_size/2, this->y + bird_size/2);
  glVertex2f(this->x - bird_size/2, this->y + bird_size/2);
  glEnd();
}

void Bird::update_pos(Obstacle * o)
{
  if (!this->has_collided(o)) {
    int t = this->time;
    float a = this->jump_acc;

    /* Jumping acceleration and speed switched off if jump_acc_time has elapsed */
    if (this->jump_acc && this->time - this->last_jump_time >= jump_acc_time) {
      this->last_jump_time = 0;
      this->time=0;
      this->jump_acc = 0;
      this->ver_speed = this->ver_speed - (g-a)*t;
    }

    float u = this->ver_speed;
    this->y += (u*t) - (0.5)*((g-a)*(t*t));
    this->time++;
    this->distance += 1;

    if (this->has_crossed(o)) {
      this->next_obs = this->next_obs->next_obs;
      ++this->score;
    }
  } else {
    this->game_over = true;
    this->total_fitness += this->distance;
    this->turns++;
    this->avg_fitness = (float)this->total_fitness / (float)this->turns;
  }
}

void Bird::jump()
{
  /* Jumping effect is added by applying an upward jump_acceleration and
   * jump_speed for jump_acc_time before turning it off */
  this->ver_speed = jump_speed;
  this->time = 2;
  this->jump_acc = jump_acceleration;
  this->last_jump_time = this->time;
}

bool Bird::has_crossed(Obstacle* o)
{
  if (o == NULL) {
    o = this->next_obs;
  }

  if ((this->x - bird_size/2) < (o->position + obs_width)) {
    return false;
  }

  return true;
}

bool Bird::has_collided(Obstacle* o)
{
  if (o == NULL) {
    o = this->next_obs;
  }

  bool collision_x = ((this->x + bird_size/2) > o->position) && \
    ((this->x - bird_size/2) < (o->position + obs_width));

  bool collision_y = ((this->y + bird_size/2) < o->opening) || \
    ((this->y - bird_size/2) > (o->opening + opening_width));

  bool collision_boundary = ((this->y + bird_size/2) > (win_height - boundary_width)) || \
    ((this->y - bird_size/2) < boundary_width);

  if ( (collision_x && collision_y) || collision_boundary ) {
    return true;
  }

  return false;
}

void Bird::reset(ObstacleList ol)
{
  this->x = bird_x;
  this->y = bird_y;
  this->score = 0;
  this->game_over = false;
  this->ver_speed = 0;
  this->time = 0;
  this->jump_acc = 0;
  this->last_jump_time = 0;
  this->distance = 0;
  this->next_obs = ol.obs_list_head;
}

NeuralBird::NeuralBird(ObstacleList ol, int num_layers, int* layers)
: Bird(ol)
{
  this->num_layers = num_layers;
  this->layers = layers;
  this->ann = fann_create_standard_array(this->num_layers, (const unsigned int*)this->layers);
  // this->total_fitness = 0;
  // this->avg_fitness = 0;
  fann_randomize_weights(this->ann, -1.0, 1.0);
}

NeuralBird::NeuralBird(NeuralBird& nb) : Bird(nb) {
  this->num_layers = nb.num_layers;
  this->layers = nb.layers;
  this->ann = fann_copy(nb.ann);
  this->total_fitness=0;
}

bool NeuralBird::should_jump(fann_type* input)
{
  static int last_time = 0;

  if(input == nullptr){
    input = this->get_dist_from_obs();
  }

  this->jump_decision = fann_run(this->ann, input);

  delete input;

  if (this->jump_decision[0] > 0.5 && last_time >= time_bw_jumps) {
    last_time = 0;
    return true;
  }
  else {
    last_time++;
    return false;
  }
}

fann_type* NeuralBird::get_dist_from_obs()
{
  int obs_pos = this->next_obs->position;
  int op_pos = this->next_obs->opening;

  fann_type* dist = new fann_type[2];
  dist[0] = obs_pos - this->x;
  dist[1] = this->y - op_pos;

  return dist;
}

struct action_value NeuralBird::calc_q(fann_type* input)
{
  fann_type* q_values = fann_run(this->ann, input);

  struct action_value av = {q_values[0], q_values[1]};
  return av;
}
