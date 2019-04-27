#include<GL/glut.h>
#include<iostream>
#include<math.h>
#include "env.h"
#include "obstacles.h"
#include "bird.h"

void display();
void keyboard(unsigned char c, int x, int y);
void render_outline();
void init();
void idle();

bool game_started = false;
// ObstacleList ol(obs_no);
ObstacleList ol;
Bird b;

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(win_width,win_height);
    glutInitWindowPosition(50,50);
    glutCreateWindow("win");
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle);
    glutMainLoop();
}

void init()
{
  gluOrtho2D(0,1000,0,500);
  ol.generate_obstacles();
  ol.display_list();
}

void display()
{
  glClearColor(0,0,0,0);
  glClear(GL_COLOR_BUFFER_BIT);
  glColor3f(1,1,1);

  render_outline();
  b.render_bird();

  // for (int i=0;i<obs_no;i++) {
  //   ol.obs_list[i]->render_obstacle();
  // }
  ol.render_obstacles();

  glutSwapBuffers();
}

void render_outline()
{
  glBegin(GL_LINES);
  // Vertical boundary
  glVertex2f(0, boundary_width);
  glVertex2f(win_width, boundary_width);

  glVertex2f(win_width, win_height - boundary_width);
  glVertex2f(0, win_height - boundary_width);

  glEnd();
}

void keyboard(unsigned char c, int x, int y)
{
  if ((int)c == SPACEBAR) {
    if (game_started) {
      b.jump();
    } else {
      game_started = true;
    }
  }
}

void idle()
{
  if (game_started) {
    ol.update_pos();
    b.update_pos();
  }
}
