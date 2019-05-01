#include <GL/glut.h>
#include <math.h>
#include "env.h"
#include "bird.h"

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
  this->next_obs = ol.obs_list_head;
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

void Bird::update_pos()
{
  if (!this->has_collided()) {
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

    if (this->has_crossed()) {
      this->next_obs = this->next_obs->next_obs;
      ++this->score;
    }

    glutPostRedisplay();
  } else {
    this->game_over = true;
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
  glutPostRedisplay();
}

bool Bird::has_crossed()
{
  if ((this->x - bird_size/2) < (this->next_obs->position + obs_width)) {
    return false;
  }

  return true;
}

bool Bird::has_collided()
{
  bool collision_x = ((this->x + bird_size/2) > this->next_obs->position) && \
    ((this->x - bird_size/2) < (this->next_obs->position + obs_width));

  bool collision_y = ((this->y + bird_size/2) < this->next_obs->opening) || \
    ((this->y - bird_size/2) > (this->next_obs->opening + opening_width));

  bool collision_boundary = ((this->y + bird_size/2) > (win_height - boundary_width)) || \
    ((this->y - bird_size/2) < boundary_width);

  if ( (collision_x && collision_y) || collision_boundary ) {
    return true;
  }

  return false;
}
