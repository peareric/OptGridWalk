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
    // Start with zero steps at the start node
    int walk_num_steps = 0;
    _walker.set_position(_grid->get_start());
    _walker.visit_grid(_grid);
    auto goal = _grid->get_goal();

    // Walk until the goal is reached or the max number of steps is met
    while (!_walker.at_coordinate(goal) && walk_num_steps < _max_steps) {
      _walker.step(_grid);
      _walker.visit_grid(_grid);
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
  mean = goal_m1 / num_samples;
  // Compute varaince of analog average
  mean_var = (goal_m2/num_samples - mean*mean) / num_samples;
  // Compute and return the figure of Merit of the walk
	if (avg_num_steps == 0) {
    throw std::runtime_error("Average number of steps is 0!");
  }
  else if (mean_var == 0) {
    throw std::runtime_error(
      "Standard deviation of average number of steps is 0!");
  }
  FOM = 1.0/(avg_num_steps*mean_var);
  return FOM;
}

void MCWalk::print_results() const {
  std::cout << std::fixed;
	std::cout << std::showpoint;
	std::cout << std::setprecision(5);
  std::cout << "mean:  " << std::setw(8) << mean << std::endl;
  std::cout << "error: " << std::setw(8) << std::sqrt(mean_var) << std::endl;
  std::cout << "FOM:   " << std::setw(8) << FOM << std::endl;
}

