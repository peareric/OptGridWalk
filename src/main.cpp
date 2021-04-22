#include "mc_walk.hpp"
#include "walk_manager.hpp"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>

int main(int argc, char* argv []) {
  // Run all Monte Carlo walks with 100,000 samples
  double num_samples = 1e6;

  // Open input file with grid description and build Grid class
  std::ifstream grid_input;
  grid_input.open(argv[1]);
  if(!grid_input.is_open()) {
    std::cout << "Failed to open "+std::string(argv[1]) << std::endl;
    return 1;
  }
  std::cout << "Reading grid specification from " << argv[1] << std::endl;
  Grid mesh_grid(grid_input);
  grid_input.close();
  std::cout << "Mesh grid read in successfully\n" << std::endl;

  // Open input file with the biased PDF parameters to simulate and build
  // monte carlo simulation manager class
  std::ifstream biased_PDF_input;
  biased_PDF_input.open(argv[2]);
  if(!biased_PDF_input.is_open()) {
    std::cout << "Failed to open "+std::string(argv[2]) << std::endl;
    return 1;
  }
  std::cout << "Reading biased PDF parameters from " << argv[2] << std::endl;
  WalkManager MC_manager(biased_PDF_input, num_samples);
  grid_input.close();
  std::cout << "PDF parameters read in successfully\n" << std::endl;

  // Run all Monte Carlo simualations, outputting results and storing FOM
  MC_manager.run_all_cases(&mesh_grid);

  std::cout << "\nAverage analog spatial distribution:" << std::endl;
  MC_manager.print_analog(std::cout);

  return 0; 
}