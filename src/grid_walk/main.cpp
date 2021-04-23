#include "walk_manager.hpp"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>

int main(int argc, char* argv []) {
  if (argc != 3) {
    std::cout << "Must pass both grid and walk parameter specification files";
    std::cout << std::endl;
    return 1;
  }

  // Open input file with grid description and build Grid class
  std::ifstream grid_input;
  std::string grid_filename(argv[1]);
  grid_input.open(grid_filename);
  if(!grid_input.is_open()) {
    std::cout << "Failed to open "+grid_filename << std::endl;
    return 2;
  }
  std::cout << "Reading grid specification from "+grid_filename << std::endl;
  Grid mesh_grid(grid_input);
  grid_input.close();
  std::cout << "Mesh grid read in successfully\n" << std::endl;

  // Open input file with the biased PMF parameters to simulate and build
  // monte carlo simulation manager class
  std::ifstream biased_PMF_input;
  std::string PMF_filename(argv[2]);
  biased_PMF_input.open(PMF_filename);
  if(!biased_PMF_input.is_open()) {
    std::cout << "Failed to open "+PMF_filename << std::endl;
    return 2;
  }
  std::cout << "Reading biased PMF parameters from "+PMF_filename << std::endl;
  WalkManager MC_manager(biased_PMF_input);
  grid_input.close();
  std::cout << "PMF parameters read in successfully\n" << std::endl;

  // Run all Monte Carlo simualations, outputting results and storing FOM
  MC_manager.execute(&mesh_grid);

  // Save the data for training
  std::ofstream output_file;
  std::string grid_name(grid_filename.begin()+12, grid_filename.end()-4);
  std::string PMF_name(PMF_filename.begin()+12, PMF_filename.end()-4);
  std::string output_filename = grid_name+"_"+PMF_name+"_results.txt";
  std::cout << output_filename << std::endl;
  output_file.open(output_filename);
  if (!output_file.is_open()) {
    // input file name deduction failed
    output_filename = "walk_results.txt";
    output_file.open(output_filename);
  }
  std::cout << "Writing PMF parameters and average steps data to ";
  std::cout <<  output_filename << std::endl;
  MC_manager.print_results(output_file);
  output_file.close();

  return 0; 
}