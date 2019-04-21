#include<GL/glut.h>
#include "obstacles.h"
#include "env.h"

//Obstacle constructors
Obstacle::Obstacle()
{
  this->position = 0;
  this->opening = 0;
}

Obstacle::Obstacle(int pos, int op)
{
  this->position = pos;
  this->opening = op;
}

void render_obstacle(Obstacle* obs)
{
  glColor3f(1,1,1);
  // Render the full obstacle leaving a small opening
  glBegin(GL_QUADS);
  glVertex2f(obs->position, boundary_width);
  glVertex2f(obs->position, obs->opening);
  glVertex2f(obs->position + obs_width, obs->opening);
  glVertex2f(obs->position + obs_width, boundary_width);
  glEnd();

  glBegin(GL_QUADS);
  glVertex2f(obs->position, obs->opening + opening_width);
  glVertex2f(obs->position, win_height - boundary_width);
  glVertex2f(obs->position + obs_width, win_height - boundary_width);
  glVertex2f(obs->position + obs_width, obs->opening + opening_width);
  glEnd();
}

void generate_obstacles(Obstacle** o)
{
  o[0] = new Obstacle(200,300);

  for (int i = 1; i < obs_no; i++)
   {
     int pos = o[i-1]->position + brick_gap;
     int op = (rand() % (win_height - boundary_width*2 - opening_width - obs_offset)) \
      + (boundary_width + obs_offset);

     o[i] = new Obstacle(pos, op);
   }
}

void update_value(Obstacle** obs) {
  for (int i=0;i<obs_no;i++) {
    obs[i]->position = obs[i]->position - obs_speed;
  }

  glutPostRedisplay();
}
