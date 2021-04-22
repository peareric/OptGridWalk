#ifndef _WALK_MANAGER_HEADER_
#define _WALK_MANAGER_HEADER_

#include "grid.hpp"
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
  
  // Save the results of the analog walk on the grid for later printing
  Grid _analog_grid;

  // Helper function to run walk and time the execuation time
  double time_walk(MCWalk & walk, int i) const;

public:
  WalkManager(std::ifstream & input_file, double num_samples);
  ~WalkManager() {};

  // Performs a Monte Carlo walk for the analog PDFs and all biased PDFs in
  // input file, save the results of the analog walk, and return a cleared grid
  void run_all_cases(Grid * grid);

  // Requires that run_all_cases has been called, returns the results of those runs
  std::vector<std::vector<double>> get_walk_data() const { return _walk_data; };

  // Print the grid of the analog simualtion
  void print_analog(std::ostream & output_file) const { 
    _analog_grid.print(output_file, _num_samples);
  }
};

#endif