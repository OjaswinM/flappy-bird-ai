#include <GL/glut.h>
#include <iostream>
#include <sstream>
#include <cstring>
#include <math.h>
#include "env.h"
#include "obstacles.h"
#include "bird.h"

void display();
void keyboard(unsigned char c, int x, int y);
void render_outline();
void init();
void idle();
void game_over();
void render_score(int score);

bool game_started = false;
// ObstacleList ol(obs_no);
ObstacleList ol;
Bird b(ol);

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(win_width,win_height);
    glutInitWindowPosition(50,50);
    glutCreateWindow("Birdy");
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle);
    glutMainLoop();
}

void init()
{
  gluOrtho2D(0,1000,0,500);
  //ol.generate_obstacles();
}

void display()
{
  if (!b.game_over) {
    glClearColor(0,0,0,0);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1,1,1);

    render_outline();
    b.render_bird();
    ol.render_obstacles();
    render_score(b.score);
  } else {
    game_over();
  }

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
  if (game_started && !b.game_over) {
    ol.update_pos();
    b.update_pos();

    glutPostRedisplay();
  }
}

void render_menu()
{
  if(b.game_over)
  {
    game_over();
  } else {

  }
}

void game_over()
{
  unsigned char go_text[] = "THANK YOU FOR PLAYING!";
  int text_width = glutBitmapLength(GLUT_BITMAP_8_BY_13, go_text);
  /* Centre in the middle of the window */
  glRasterPos2i(win_width/2 - text_width/2, 470);
  /* Render the string */
  int len = strlen((const char*)go_text);
  for (int i = 0; i < len; i++) {
    glutBitmapCharacter(GLUT_BITMAP_8_BY_13, go_text[i]);
  }
}

void render_score(int score)
{
  unsigned char score_text[] = "Score: ";
  std::ostringstream digit;
  digit<<score;
  std::string temp_score = digit.str();
  char * score_char = (char *) temp_score.c_str();
  strcat((char *)score_text, score_char);

  int text_width = glutBitmapLength(GLUT_BITMAP_8_BY_13, score_text);
  /* Centre in the middle of the window */
  glRasterPos2i(800, 470);
  /* Render the string */
  int len = strlen((const char*)score_text);
  for (int i = 0; i < len; i++) {
    glutBitmapCharacter(GLUT_BITMAP_8_BY_13, score_text[i]);
  }
}
