#include "mc_walk.hpp"

#include <chrono>
#include <fstream>
#include <iostream>

int main(int argc, char* argv []) {
  // Open input file with grid description
  std::ifstream grid_input;
  grid_input.open(argv[1]);
  std::cout << "Reading grid specification from " << argv[1] << std::endl;
  Grid mesh_grid(grid_input);
  std::cout << "Mesh grid read in successfully\n\n" << std::endl;

  // Run the analog random walk with N samples
  auto start = std::chrono::steady_clock::now();
  double N = 10e7;
  MCWalk monte_carlo_walk(&mesh_grid);
  std::cout << "Running analog random walk with " << N << " samples" << std::endl;
  monte_carlo_walk.walk_grid(N);
  double FOM = monte_carlo_walk.get_FOM();
  std::cout << "Analog simulation has a FOM of: " << FOM << std::endl;
  auto end = std::chrono::steady_clock::now();
  std::cout << "Clock time: "
            << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
            << " ms" << std::endl;

  // Print grid heatmap to input_file.out

  return 0; 
}