#ifndef __MC_WALK_HEADER__
#define __MC_WALK_HEADER__

#include "grid.hpp"
#include "walker.hpp"

#include <cmath>
#include <fstream>

// Class governing Monte Carlo random walk through the grid
class MCWalk {
private:
  Grid * _grid;
  Walker _walker;
  // Estimate of the mean number of analog steps to goal
  double mean = 0;
  // Estimate of the varinace the mean
  double mean_var = 0;
  // Figure of merit of the simulation
  double FOM = 0;
  // Hard coded bail out number of steps for impossible walks
  const double _max_steps = 10000;
public:
  MCWalk(Grid * grid) : _grid(grid) {};
  ~MCWalk() {};

  // Set the probability of traveling in a given direction
  // Direction ordering is clockwise starting at north
  void set_direction_PDF(const std::vector<double> &probabilties);

  // Set the parameter of the truncated exponential governing step length
  void set_length_PDF(const double lambda);

  // Perform Monte Carlo random walk on the grid num_samples times and return
  // the figure of merit, defined as 1 over the average number of steps taken
  // from start to goal (T) times the variance of the estimate of the analog
  // number of steps to the goal (R^2)
  double walk_grid(double num_samples = 1e7);

  // Requires that walk_grid has already been called, returns the estimate of
  // the analog number of steps to the goal corresponding to the last walk and
  // and estimate of the associated error
  std::pair<double, double> get_estimate() const {
    return std::pair<double, double>(mean, std::sqrt(mean_var));
  }

  // Prints the return of get_estimate as well as the figure of merit
  void print_results() const;
};

#endif