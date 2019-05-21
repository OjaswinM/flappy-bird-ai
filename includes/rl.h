#include <string>
#include <unordered_map>
#include "fann.h"
#include "bird.h"
#include "obstacles.h"

#ifndef RL_H
#define RL_H


class QTableKey
{
public:
  int inp_x, inp_y, pos_y;
  bool jump;

  QTableKey(int inp_x, int inp_y, int pos_y, bool jump);
  QTableKey(QTableKey &qtk);
  std::string to_string();
};

class RL
{
public:
  NeuralBird nb;
  ObstacleList ol;
  int num_layers, *layers;
  std::unordered_map<std::string, float> q_table;

  RL(ObstacleList ol, int num_layers, int* layers);
  void map_insert(QTableKey qtk, float q_val);
  bool map_is_present(QTableKey qtk);
  float map_get_value(QTableKey qtk);
  float calc_q_val(QTableKey qtk);
  float calc_reward(QTableKey qtk);
  float calc_max_q(QTableKey qtk);
  fann_type* calc_actual_q(QTableKey qtk);
  void train();
  QTableKey* calc_next_state(QTableKey qtk);
};

#endif
