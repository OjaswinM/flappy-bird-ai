#include<GL/glut.h>
#include "obstacles.h"
#include "env.h"
#include <time.h>

//Obstacle constructors
Obstacle::Obstacle()
{
  this->position = 0;
  this->opening = 0;
}

Obstacle::Obstacle(int pos, int op)
{
  this->init_position = pos;
  this->position = pos;
  this->opening = op;
  this->next_obs = NULL;
  this->prev_obs = NULL;
}

void Obstacle::reset()
{
  this->position = this->init_position;
}

void Obstacle::update_pos()
{
  this->position -= obs_speed;
}

void Obstacle::render_obstacle()
{
  glColor3f(1,1,1);
  // Render the full obstacle leaving a small opening
  glBegin(GL_QUADS);
  glVertex2f(this->position, boundary_width);
  glVertex2f(this->position, this->opening);
  glVertex2f(this->position + obs_width, this->opening);
  glVertex2f(this->position + obs_width, boundary_width);
  glEnd();

  glBegin(GL_QUADS);
  glVertex2f(this->position, this->opening + opening_width);
  glVertex2f(this->position, win_height - boundary_width);
  glVertex2f(this->position + obs_width, win_height - boundary_width);
  glVertex2f(this->position + obs_width, this->opening + opening_width);
  glEnd();
}

ObstacleList::ObstacleList()
{
  this->obs_list_head = NULL;
  this->obs_list_rear = NULL;
  this->generate_obstacles();
}

void ObstacleList::generate_obstacles()
{
  srand(time(0));
  for (int i = 0; i < obs_no; i++)
   {
     int pos;
     /* Random position for opening */
     int op = (rand() % (win_height - (boundary_width*2+60) - opening_width - obs_offset)) \
     + (boundary_width + obs_offset);

     if ( this->obs_list_head == NULL ) {
       pos = obs_start_pos;
     }
     else {
       pos = this->obs_list_rear->position + obs_gap;
     }

      this->add_rear(new Obstacle(pos, op));
   }
}

void ObstacleList::update_pos()
{
  Obstacle *it;   // iterator
  it = this->obs_list_head;

  while (it != NULL) {
    it->update_pos();
    it = it->next_obs;
  }
}

void ObstacleList::add_rear(Obstacle *obs)
{
  /* add obstacles to the rear */
  if ( this->obs_list_head == NULL ) {
    this->obs_list_rear = obs;
    this->obs_list_head = obs;
  } else {
    /* add backward link to obs */
    obs->next_obs = NULL;
    obs->prev_obs = this->obs_list_rear;
    /* add forward link to obs and readjust rear */
    this->obs_list_rear->next_obs = obs;
    this->obs_list_rear = obs;
  }
}

void ObstacleList::render_obstacles()
{
  Obstacle *it;
  it = this->obs_list_head;

  while(it != NULL)
  {
    it->render_obstacle();
    it = it->next_obs;
  }
}

void ObstacleList::display_list()
{
  Obstacle *it;
  it = this->obs_list_head;
  int count=0;
  while(count < 5)
  {
    it = it->next_obs;
    count++;
  }
}

void ObstacleList::reset_list()
{
  //srand(time(0));
  Obstacle *obs;
  obs = this->obs_list_head;

  while(obs != NULL)
  {
    int op = (rand() % (win_height - boundary_width*2 - opening_width - obs_offset)) \
    + (boundary_width + obs_offset);
    obs->reset();
    obs->opening = op;
    obs = obs->next_obs;
  }
}
