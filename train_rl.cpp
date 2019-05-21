#include <iostream>
#include "rl.h"
#include "bird.h"
#include "obstacles.h"
#include "fann.h"

int main()
{
  QTableKey q(1,2,3, true);
  ObstacleList ol;
  int layers[] = {3, 6, 2};
  ol.generate_obstacles();
  RL rl(ol, 3, layers);

  rl.train();

}
