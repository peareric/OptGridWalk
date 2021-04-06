#ifndef __MC_WALK_HEADER__
#define __MC_WALK_HEADER__

#include "grid.hpp"
#include "walker.hpp"

#include <fstream>

// Class governing Monte Carlo random walk through the grid
class MCWalk {
private:
  Grid * _grid;
  Walker _walker;
  unsigned int _num_samples, _num_steps;
  double _goal_m1, _goal_m2;
  double _max_steps = 10000;
public:
  MCWalk(Grid * grid) : _grid(grid) {};
  ~MCWalk() {};

  // Set the probability of traveling in a given direction
  // Direction ordering is clockwise starting at north
  void set_direction_PDF(const std::vector<double> &probabilties);

  // Set the parameter of the truncated exponential governing step length
  void set_length_PDF(const double lambda);

  // Perform Monte Carlo random walk on the grid num_samples times
  // each time adding number of steps taken from start to goal to num_steps,
  // the first goal moment to goal_m1, and the second goal moment to goal_m2
  void walk_grid(double num_samples = 10e7);

  // Perform many random walks to compute the figure of merit, defined as 1
  // over the average number of steps taken from start to goal times the
  // variance of the estimate of the goal
  double get_FOM();
};

#endif