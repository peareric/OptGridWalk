#include "walk_manager.hpp"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>

int main(int argc, char* argv []) {
  // Run all Monte Carlo walks with 10,000 samples
  double num_samples = 1e4;

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

  // Open input file with the biased PDF parameters to simulate and build
  // monte carlo simulation manager class
  std::ifstream biased_PDF_input;
  std::string PDF_filename(argv[2]);
  biased_PDF_input.open(PDF_filename);
  if(!biased_PDF_input.is_open()) {
    std::cout << "Failed to open "+PDF_filename << std::endl;
    return 2;
  }
  std::cout << "Reading biased PDF parameters from "+PDF_filename << std::endl;
  WalkManager MC_manager(biased_PDF_input, num_samples);
  grid_input.close();
  std::cout << "PDF parameters read in successfully\n" << std::endl;

  // Run all Monte Carlo simualations, outputting results and storing FOM
  MC_manager.run_all_cases(&mesh_grid);

  // Save the data for training
  std::ofstream output_file;
  std::string grid_name(grid_filename.begin()+12, grid_filename.end()-4);
  std::string PDF_name(PDF_filename.begin()+12, PDF_filename.end()-4);
  std::string output_filename = grid_name+"_"+PDF_name+"_results.txt";
  std::cout << output_filename << std::endl;
  output_file.open(output_filename);
  if (!output_file.is_open()) {
    // input file name deduction failed
    output_filename = "walk_results.txt";
    output_file.open(output_filename);
  }
  std::cout << "Writing PDF parameter and average steps data to ";
  std::cout <<  output_filename << std::endl;
  MC_manager.print_results(output_file);
  output_file.close();

  return 0; 
}