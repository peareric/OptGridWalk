#include "mc_walk.hpp"

#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
#include <stdexcept>

double MCWalk::walk_grid(double num_samples) {
  // Accumulator for the first moment of the analog number of steps to the
  // goal
  unsigned int goal_num_steps = 0;
  // Accumulator for the first moment of the weighted number of steps to the
  // goal
  double goal_m1 = 0;
  // Accumulator for the second moment of the weighted number of steps to the
  // goal 
  double goal_m2 = 0;

  // Walk the grid
  for (unsigned int i = 0; i < num_samples; i++) {
    // Reset the weight of the walker each time through the grid
    _walker.reset_weight();
    // Start with zero steps at the start node
    int walk_num_steps = 0;
    _walker.set_position(_grid->get_start());
    if (_track_grid) { _walker.visit_grid(_grid); }
    auto goal = _grid->get_goal();

    // Walk until the goal is reached or the max number of steps is met
    while (!_walker.at_coordinate(goal) && walk_num_steps < _max_steps) {
      _walker.step(_grid);
      if (_track_grid) { _walker.visit_grid(_grid); }
      ++walk_num_steps;
    }

    // Check if the goal was reached and score accumulators
    if (_walker.at_coordinate(goal)) {
      goal_num_steps += walk_num_steps;
      double weighted_steps = _walker.get_weight()*walk_num_steps;
      goal_m1 += weighted_steps;
      goal_m2 += weighted_steps*weighted_steps;
    }
    else {
      // If the max steps stop and return a FOM of zero
      std::cout << "Max number of steps exceeded on sample " << i << std::endl;
      return 0;
    }
  }

  // Average the number of steps
  double avg_num_steps = goal_num_steps / num_samples;
  // Average the analog number of steps
  _mean = goal_m1 / num_samples;
  // Compute varaince of analog average
  _mean_var = (goal_m2/num_samples - _mean*_mean) / num_samples;
  // Compute and return the figure of Merit of the walk
	if (avg_num_steps == 0) {
    throw std::runtime_error("Average number of steps is 0!");
  }
  else if (_mean_var == 0) {
    throw std::runtime_error(
      "Standard deviation of average number of steps is 0!");
  }
  _FOM = 1.0/(avg_num_steps*_mean_var);
  return _FOM;
}

void MCWalk::print_results() const {
  std::cout << std::fixed;
	std::cout << std::showpoint;
	std::cout << std::setprecision(5);
  std::cout << "mean:  " << std::setw(8) << _mean << std::endl;
  std::cout << "error: " << std::setw(8) << std::sqrt(_mean_var) << std::endl;
  std::cout << "FOM:   " << std::setw(8) << _FOM << std::endl;
}

