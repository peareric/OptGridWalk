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
  bool _track_grid;
  // Estimate of the mean number of analog steps to goal
  double _mean = 0;
  // Estimate of the varinace the mean
  double _mean_var = 0;
  // Figure of merit of the simulation
  double _FOM = 0;
  // Hard coded bail out number of steps for impossible walks
  const double _max_steps = 10000;
public:
  MCWalk(Grid * grid, bool track_grid = false)
    : _grid(grid), _track_grid(track_grid) {};
  ~MCWalk() {};

  // Prepares for a repeated walk
  void reset() {
    _walker.reset();
    _mean = 0;
    _mean_var = 0;
    _FOM = 0;
  }

  // Set the PDF to biased values
  // Probabilities are assumed to be set in the following order:
  // [direction PDF] [distance PDF]
  // Direction parameters are passed in the order:
  // north, north_east, east, south_east,
  // south, south_west, west, north_west
  void set_biased_PDF(const std::vector<double> &probabilities) {
    _walker.set_biased_PDF(probabilities);
  }

  // Perform Monte Carlo random walk on the grid num_samples times and return
  // the figure of merit, defined as 1 over the average number of steps taken
  // from start to goal (T) times the variance of the estimate of the analog
  // number of steps to the goal (R^2)
  double walk_grid(double num_samples = 1e7);

  // Requires that walk_grid has already been called, returns the estimate of
  // the analog number of steps to the goal corresponding to the last walk,
  // and estimate of the associated error, and the figure of merit
  std::vector<double> get_results() const {
    return std::vector<double>({_mean, std::sqrt(_mean_var), _FOM});
  }

  // Requires that walk_grid has already been called, returns the figure of
  // merit of the last walk
  double get_FOM() const { return _FOM; }

  // Prints the return of get_estimate as well as the figure of merit
  void print_results() const;
};

#endif