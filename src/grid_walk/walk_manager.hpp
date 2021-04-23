#ifndef _WALK_MANAGER_HEADER_
#define _WALK_MANAGER_HEADER_

#include "util/dist.hpp"
#include "util/grid.hpp"
#include "mc_walk.hpp"

#include <vector>

// Class to perform repeated Monte Carlo simulations to produce training data
class WalkManager {
private:
  // Probability distributions class
  util::PDF _prob_distributions;
  // Vector of parameters and results from each walk performed stored as:
  // [biased_direction_pmf, biased_distance_pmf, FOM]
  // Where direction information is stored in the order:
  // north, north_east, east, south_east,
  // south, south_west, west, north_west
  std::vector<std::vector<double>> _walk_data;
  // Number of times to sample each random walk
  double _num_samples;
  // Number of times to evaluate the function if performing simulated annealing
  double _num_evals;
  // Whether of not simulation is an optimization
  bool _optimize;
  // Boolean whether or not to print the spatial distributions of each walk
  bool _print_grids;

  // Helper function to run walk and time the execuation time
  double time_walk(MCWalk & walk, int i) const;

  // Performs a Monte Carlo walk for the analog PMFs and all biased PMFs in
  // input file, save the results in _walk_data, and returns a cleared grid
  void run_all_cases(Grid * grid);

  // Performs simulated annealing to determing optimal PMF parameters resulting
  // in the shortest walk from the start to the goal.
  void simulate_annealing(Grid * grid);

public:
  WalkManager(std::ifstream & input_file);
  ~WalkManager() {};

  // Calls either run_all_cases or simulate_annealing depending on user input
  void execute(Grid * grid);

  // Print biased PMF parameters and resulting mean number of steps
  void print_results(std::ofstream & output_file) const;
};

#endif