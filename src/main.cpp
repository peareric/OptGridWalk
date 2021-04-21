#include "mc_walk.hpp"

#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>

int main(int argc, char* argv []) {
  // Open input file with grid description
  std::ifstream grid_input;
  grid_input.open(argv[1]);
  std::cout << "Reading grid specification from " << argv[1] << std::endl;
  Grid mesh_grid(grid_input);
  std::cout << "Mesh grid read in successfully\n\n" << std::endl;

  // Run the analog random walk with 10^7 samples
  double num_samples = 1e7;
  MCWalk monte_carlo_walk(&mesh_grid);
  auto start = std::chrono::steady_clock::now();
  std::cout << "Running analog random walk with " << num_samples << " samples";
  std::cout << std::endl;
  double FOM = monte_carlo_walk.walk_grid(num_samples);
  auto end = std::chrono::steady_clock::now();
  std::cout << "Clock time: "
            << std::chrono::duration_cast<std::chrono::milliseconds>(
              end - start).count() << " ms" << std::endl;
  std::cout << "Analog simulation results:" << std::endl;
  monte_carlo_walk.print_results();

  std::cout << "Average analog spatial distribution:" << std::endl;
  mesh_grid.print(std::cout, num_samples);

  return 0; 
}