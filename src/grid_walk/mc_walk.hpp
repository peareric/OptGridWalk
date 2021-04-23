#ifndef __MC_WALK_HEADER__
#define __MC_WALK_HEADER__

#include "util/grid.hpp"
#include "walker.hpp"

#include <cmath>
#include <fstream>

// Class governing Monte Carlo random walk through the grid
class MCWalk {
private:
  // Pointer to the grid being walked on
  Grid * _grid;
  // Walker object to move on grid
  Walker _walker;
  // Whether of not to update visits on the grid as walk progresses
  bool _track_grid;
  // Average number of steps taken to goal
  double _num_steps = 0;
  // Estimate of the mean number of analog steps to goal
  double _mean = 0;
  // Estimate of the varinace the mean
  double _mean_var = 0;
  // Figure of merit of the simulation
  double _FOM = 0;
  // Hard coded bail out number of steps for impossible walks
  const double _max_steps = 100000;
  // Lowest acceptable weight
  // [no longer used, importance sampling commented out]
  const double _min_wight = 1e-7;

public:
  MCWalk(Grid * grid, bool track_grid = false)
    : _grid(grid), _track_grid(track_grid) {};
  ~MCWalk() {};

  // Prepares for a repeated walk
  void reset() {
    _walker.reset();
    _num_steps = 0;
    _mean = 0;
    _mean_var = 0;
    _FOM = 0;
  }

  // Set the PMF to biased values
  // Probabilities are assumed to be set in the following order:
  // [direction PMF] [distance PMF]
  // Direction parameters are passed in the order:
  // north, north_east, east, south_east,
  // south, south_west, west, north_west
  void set_biased_PMF(const std::vector<double> &probabilities) {
    _walker.set_biased_PMF(probabilities);
  }

  // Perform Monte Carlo random walk on the grid num_samples times and return
  // the average number of steps taken to get to the goal per history
  double walk_grid(double num_samples = 1e7);

  // Prints the return of get_estimate as well as the figure of merit
  void print_results() const;

  // Print the PMF paramters of teh walker
  void print_walker() const { _walker.print_PMF_paramters(); }
};

#endif