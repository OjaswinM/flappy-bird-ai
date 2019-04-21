#include<GL/glut.h>
#include<iostream>
#include<math.h>
#include "env.h"
#include "obstacles.h"

void display();
void render_outline();
// void render_obstacle(struct obstacle obs);
void my_init();
// void generate_obstacles();
//void lolith(struct obstacle o);
// void update_value();

// struct obstacle {
//   int position;
//   int opening;
// };
//
// struct obstacle o[obs_no];

//using namespace std;

Obstacle **o = new Obstacle *[obs_no];

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(win_width,win_height);
    glutInitWindowPosition(50,50);
    glutCreateWindow("win");
    my_init();
    glutDisplayFunc(display);
    glutMainLoop();
}

void my_init()
{
  gluOrtho2D(0,1000,0,500);
  generate_obstacles(o);
}

void display()
{
  glClearColor(0,0,0,0);
  glClear(GL_COLOR_BUFFER_BIT);
  glColor3f(1,1,1);
  render_outline();

  for (int i=0;i<obs_no;i++) {
    render_obstacle(o[i]);
  }
  glutSwapBuffers();
  update_value(o);
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

// void render_obstacle(struct obstacle obs)
// {
//   glColor3f(1,1,1);
//   // Render the full obstacle
//   glBegin(GL_QUADS);
//   glVertex2f(obs.position, boundary_width);
//   glVertex2f(obs.position, win_height - boundary_width);
//   glVertex2f(obs.position + obs_width, win_height - boundary_width);
//   glVertex2f(obs.position + obs_width, boundary_width);
//   glEnd();
//
//   // Randomly determine the opening coordinates
//    //int opening = rand() % (win_height - boundary_width*2 - opening_width);
//   // opening = opening + 80;
//
//   glColor3f(0, 0, 0);
//   // Create the opeining
//   glBegin(GL_QUADS);
//   glVertex2f(obs.position, obs.opening);
//   glVertex2f(obs.position, obs.opening + opening_width);
//   glVertex2f(obs.position + obs_width, obs.opening + opening_width);
//   glVertex2f(obs.position + obs_width, obs.opening);
//   glEnd();
// }

// void generate_obstacles()
// {
//   o[0].position=200;
//   o[0].opening=300;
//   //lolith(o[0]);
//   for (int i = 1; i < obs_no; i++)
//    {
//      o[i].position = o[i-1].position + brick_gap;
//      o[i].opening=(rand() % (win_height - boundary_width*2 - opening_width - obs_offset)) \
//       + (boundary_width + obs_offset);
//      //cout << i << " ";
//    }
// }

// void lolith(struct obstacle o)
// {
//   std::cout << "(pos: " << o.position << ", op: " << o.opening << ")" << std::endl;
// }

// void update_value() {
//   for (int i=0;i<obs_no;i++) {
//     o[i].position = o[i].position - obs_speed;
//   }
//
//   glutPostRedisplay();
// }
