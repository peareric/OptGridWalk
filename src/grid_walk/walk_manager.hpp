#ifndef _WALK_MANAGER_HEADER_
#define _WALK_MANAGER_HEADER_

#include "util/grid.hpp"
#include "mc_walk.hpp"

#include <vector>

// Class to perform repeated Monte Carlo simulations to produce training data
class WalkManager {
private:
  // Vector of parameters and results from each walk performed stored as:
  // [biased_direction_pdf, biased_distance_pdf, FOM]
  // Where direction information is stored in the order:
  // north, north_east, east, south_east,
  // south, south_west, west, north_west
  std::vector<std::vector<double>> _walk_data;
  
  // Number of times to sample each random walk
  const double _num_samples;
  
  // Boolean whether or not to print the spatial distributions of each walk
  bool _print_grids;

  // Helper function to run walk and time the execuation time
  double time_walk(MCWalk & walk, int i) const;

public:
  WalkManager(std::ifstream & input_file, double num_samples);
  ~WalkManager() {};

  // Performs a Monte Carlo walk for the analog PDFs and all biased PDFs in
  // input file, save the results of the analog walk, and return a cleared grid
  void run_all_cases(Grid * grid);

  // Print biased PDF parameters and resulting FOM
  void print_results(std::ofstream & output_file) const;
};

#endif