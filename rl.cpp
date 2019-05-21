#include <unordered_map>
#include "rl.h"
#include "env.h"
#include <cmath>
#include <iostream>

QTableKey::QTableKey(int inp_x, int inp_y, int pos_y, bool jump)
{
  this->inp_x = inp_x;
  this->inp_y = inp_y;
  this->pos_y = pos_y;
  this->jump = jump;
}

QTableKey::QTableKey(QTableKey &qtk)
{
  this->inp_x = qtk.inp_x;
  this->inp_y = qtk.inp_y;
  this->pos_y = qtk.pos_y;
  this->jump = qtk.jump;
}

std::string QTableKey::to_string()
{
  return std::to_string(this->inp_x) + " " \
    + std::to_string(this->inp_y) + " " \
    + std::to_string(this->pos_y) + " " \
    + std::to_string(this->jump);
}

RL::RL(ObstacleList ol, int num_layers, int* layers)
  : nb(ol, num_layers, layers), ol(ol), num_layers(num_layers), layers(layers)
{}

void RL::map_insert(QTableKey qtk, float q_val)
{
  // std::pair<std::string, float> entry (qtk.to_string(), q_val);
  //
  // this->q_table.insert(entry);
  this->q_table[qtk.to_string()] = q_val;
}

bool RL::map_is_present(QTableKey qtk)
{
  std::unordered_map<std::string,float>::const_iterator res = this->q_table.find (qtk.to_string());

  if (res == this->q_table.end())
    return false;

  return true;
}

float RL::map_get_value(QTableKey qtk)
{
  std::unordered_map<std::string,float>::const_iterator res = this->q_table.find (qtk.to_string());

  return res->second;
}

float RL::calc_q_val(QTableKey qtk)
{
  // if (!this->map_is_present(qtk)) {
  //   this->map_insert(qtk, 0.0);
  //   return 0.0;
  // }
  //
  // float reward = this->calc_reward(qtk);
  // QTableKey new_state = this->calc_next_state(qtk);
  // float max_q = this->calc_max_q(new_state);
  //
  // float q = (1 - learn_rate) * this->map_get_value(qtk) + learn_rate * (reward + disc_factor * max_q);
  // return q;
}

float RL::calc_max_q(QTableKey qtk)
{
  // qtk.jump = 0;
  // float q1 = this->calc_q_val(qtk);
  //
  // qtk.jump = 1;
  // float q2 = this->calc_q_val(qtk);
  //
  // return (q1>q2 ? q1 : q2);
  fann_type* input = new fann_type[3];
  input[0] = qtk.inp_x;
  input[1] = qtk.inp_y;
  input[2] = qtk.pos_y;

  struct action_value av = this->nb.calc_q(input);

  return av.jump > av.no_jump ? av.jump : av.no_jump;
}

float RL::calc_reward(QTableKey qtk)
{
  NeuralBird nb(this->nb);
  int init_score = nb.score;

  Obstacle o(nb.next_obs->position, nb.next_obs->opening);

  if(qtk.jump)
    nb.jump();

  for (int i=0; i<time_bw_jumps; i++) {
    o.update_pos();
    nb.update_pos(&o);
  }

  if(nb.game_over)
    return 0.0;
  else if(nb.score > init_score)
    return 1.0;
  else
    return 0.5;
}

QTableKey* RL::calc_next_state(QTableKey qtk)
{
  NeuralBird nb(this->nb);
  Obstacle obs[2];
  Obstacle* it = nb.next_obs;

  for (int i=0; i<2; i++) {
    obs[i].opening = it->opening;
    obs[i].position = it->position;
    it = it->next_obs;
  }

  obs[0].next_obs = &obs[1];
  nb.next_obs = &obs[0];
  for (int i=0; i<time_bw_jumps; i++) {
    obs[0].update_pos();
    obs[1].update_pos();
    nb.update_pos();
  }

  fann_type* dist = nb.get_dist_from_obs();
  QTableKey* new_state = new QTableKey((int)dist[0], (int)dist[1], nb.y, false);
  return new_state;
}

fann_type* RL::calc_actual_q(QTableKey qtk)
{
  fann_type *out = new fann_type[2];

  float reward = this->calc_reward(qtk);

  QTableKey* next_state;
  next_state = this->calc_next_state(qtk);

  fann_type *input = new fann_type[3];
  input[0] = next_state->inp_x;
  input[1] = next_state->inp_y;
  input[2] = next_state->pos_y;

  struct action_value av = this->nb.calc_q(input);

  out[0] = reward + disc_factor * av.no_jump;
  out[1] = reward + disc_factor * av.jump;

  return out;
}

void RL::train()
{
  int max = 0;
  for(int i=0; i<1000000; i++) {
    this->ol.reset_list();
    this->nb.reset(this->ol);
    while (!(this->nb.game_over)) {
      /* input for the ann */
      bool jump = 0;
      fann_type *input = new fann_type[3];
      input = this->nb.get_dist_from_obs();
      input[2] = this->nb.y;
      QTableKey qtk((int)input[0], (int)input[1], (int)input[2], jump);

      struct action_value av = this->nb.calc_q(input);

      if (av.jump > av.no_jump) {
        this->nb.Bird::jump();
        qtk.jump = true;
      } else {
        qtk.jump = false;
      }

      fann_type *out = this->calc_actual_q(qtk);

      // if (qtk.jump) {
      //   out[0] = av.no_jump;
      // } else {
      //   out[1] = av.jump;
      // }

      fann_train(this->nb.ann, input, out);

      delete out;

      for(int i=0; i<time_bw_jumps; i++)
      {
        this->ol.update_pos();
        this->nb.update_pos();
      }
    }
    if (this->nb.distance > max)
      max = this->nb.distance;

    std::cout << "Score: " << this->nb.score << " Distance: " << this->nb.distance << " Highscore: " << max << "\n";
  }
}
