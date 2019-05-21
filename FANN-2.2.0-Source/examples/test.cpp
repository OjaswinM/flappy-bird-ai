#include "fann.h"
#include <iostream>

int main()
{
  fann_type *calc_out, inp[2];
  struct fann *ann;
  const unsigned int num_input = 2;
  const unsigned int num_output = 1;
  const unsigned int num_layers = 3;
  const unsigned int num_neurons_hidden = 6;

  ann = fann_create_standard(num_layers, num_input, num_neurons_hidden, num_output);

  fann_randomize_weights(ann, -1.0, 1.0);

  inp[0]=20;
  inp[1]=-40;
  calc_out = fann_run(ann, inp);
  fann_type* weights = new fann_type[20];
  fann_connection *conn = new fann_connection[fann_get_total_connections(ann)];
  fann_get_connection_array(ann, conn);
  fann_print_connections(ann);

  //unsigned int* bias = new unsigned int*[fann_num_layers(ann)];
  //fann_get_bias_array(ann, bias);

  for(int i=0; i< num_layers; i++) {
    std::cout << conn[i].from_neuron << std::endl;
  }
}
