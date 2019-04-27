#include <GL/glut.h>
#include <math.h>
#include "env.h"
#include "bird.h"

Bird::Bird()
{
  this->x = bird_x;
  this->y = bird_y;
  this->ver_speed = 0;
  this->time = 0;
  this->jump_acc = 0;
  this->last_jump_time = 0;
  // this->next_obs = ol.obs_list[0];
}

void Bird::render_bird()
{
  glColor3f(1, 1, 1);

  glBegin(GL_QUADS);
  glVertex2f(this->x - bird_size/2, this->y - bird_size/2);
  glVertex2f(this->x + bird_size/2, this->y - bird_size/2);
  glVertex2f(this->x + bird_size/2, this->y + bird_size/2);
  glVertex2f(this->x - bird_size/2, this->y + bird_size/2);
  glEnd();
}

void Bird::update_pos()
{
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

    glutPostRedisplay();
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
  if (this->x < (this->next_obs->position + obs_width)) {
    return false;
  }

  return true;
}
