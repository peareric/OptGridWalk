#include "mc_walk.hpp"

void MCWalk::walk_grid(double num_samples) {
  _num_samples = num_samples;
  for (unsigned int i = 0; i < num_samples; i++) {
    int num_steps = 0;
    _walker.set_position(_grid->get_start());
    for(unsigned int j = 0; j < _max_steps; j++) {
      util::direction dir = 
    }
  }
}