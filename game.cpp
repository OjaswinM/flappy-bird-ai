#include <GL/glut.h>
#include <iostream>
#include <sstream>
#include <cstring>
#include <math.h>
#include "env.h"
#include "obstacles.h"
#include "bird.h"
#include "fann.h"
#include <time.h>

void display();
void keyboard(unsigned char c, int x, int y);
void render_outline();
void init();
void idle();
void game_over();
void update_positions();
void render_score(int score);

bool game_started = false;
bool ai_mode = false;
// ObstacleList ol(obs_no);
ObstacleList ol;
Bird b(ol);
int layers[] = {2, 6, 1};
NeuralBird nb(ol, 3, layers);

int main(int argc, char** argv)
{

  srand(time(0));
    struct fann* ann = fann_create_from_file("trained_network_bak");
    nb.ann = fann_copy(ann);
    glutInit(&argc, argv);
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(win_width,win_height);
    glutInitWindowPosition(50,50);
    glutCreateWindow("Birdy");
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    //glutIdleFunc(idle);
    glutMainLoop();
}

void init()
{
  gluOrtho2D(0,1000,0,500);
  //ol.generate_obstacles();
}

void display()
{
  if (!ai_mode) {
    if (!b.game_over) {
      glClearColor(0,0,0,0);
      glClear(GL_COLOR_BUFFER_BIT);
      glColor3f(1,1,1);
      b.score=12;
      render_outline();
      b.render_bird();
      ol.render_obstacles();
      update_positions();
      //render_score(b.score);
    } else {
      game_over();
    }
  } else {
    if (!nb.game_over) {
      glClearColor(0,0,0,0);
      glClear(GL_COLOR_BUFFER_BIT);
      glColor3f(1,1,1);

      render_outline();
      nb.Bird::render_bird();
      ol.render_obstacles();
      update_positions();
      render_score(nb.score);
    } else {
      game_over();
    }
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
  } else if (!game_started && c == 'q') {
    ai_mode = true;
    game_started = true;
  }
}

void update_positions()
{
  if (!ai_mode) {
    if (game_started && !b.game_over) {
      ol.update_pos();
      b.update_pos();
    }
  } else {
    if (game_started && !nb.game_over) {
      ol.update_pos();
      nb.Bird::update_pos();

      if(nb.should_jump())
        nb.Bird::jump();
    }
  }

  glutPostRedisplay();
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
  int score_digits_rev[10], i=0, num_digit=0;
  int temp = score;
  while (temp) {
    score_digits_rev[i++] = temp % 10;
    temp /= 10;
    ++num_digit;
  }

  for (int i=num_digit-1; i>=0; --i) {
    std::ostringstream digit;
    digit<<score_digits_rev[i];
    std::string temp_score = digit.str();
    char * score_char = (char *) temp_score.c_str();
    strcat((char *)score_text, score_char);
  }

  int text_width = glutBitmapLength(GLUT_BITMAP_8_BY_13, score_text);
  /* Centre in the middle of the window */
  glRasterPos2i(800, 470);
  /* Render the string */
  int len = strlen((const char*)score_text);
  for (int i = 0; i < len; i++) {
    glutBitmapCharacter(GLUT_BITMAP_8_BY_13, score_text[i]);
  }
}
