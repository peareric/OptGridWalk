#include "walk_manager.hpp"

#include <algorithm>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>

// Reads in the biased PDFs to simulate and saves them to the results vector
// File format is
// Entries N
// Print Spatial Distributions [0/1]
// [direction pdf values] [distance pdf value]
// Where 
// N is the number of biased PDF entries
// [0/1]
//Direction information is stored in the order:
// north, north_east, east, south_east, south, south_west, west, north_west
WalkManager::WalkManager(std::ifstream & input_file, double num_samples) 
    : _num_samples(num_samples) {
  std::string junk;
  int num_entries;
  input_file >> junk >> num_entries;
  input_file >> junk >> junk >> junk >> _print_grids;
  _walk_data.reserve(num_entries+1);
  // Manually add analog case first
  _walk_data.push_back(std::vector<double>(
    {0.125,0.125,0.125,0.125,0.125,0.125,0.125,0.125,1.0}));
  for (int i = 0; i < num_entries; i++) {
    double val;
    std::vector<double> row_data;
    for (int j = 0; j < 9; j++) {
      input_file >> val;
      row_data.push_back(val);
    }
    _walk_data.push_back(row_data);
  }
}

double WalkManager::time_walk(MCWalk & walk, int i) const {
  std::cout << "Starting walk " << i << std::endl;
  auto start = std::chrono::steady_clock::now();
  double FOM = walk.walk_grid(_num_samples);
  auto end = std::chrono::steady_clock::now();
  std::cout << "Random walk complete" << std::endl;
  std::cout << "Clock time: "
            << std::chrono::duration_cast<std::chrono::seconds>(
              end - start).count() << " sec" << std::endl;
  walk.print_results();
  return FOM;
}

void WalkManager::run_all_cases(Grid * grid) {
  std::cout << "Running " << _walk_data.size() << " random walks with ";
  std::cout << _num_samples << " samples each" << std::endl;
  std::cout << "Walk 0 is analog walk\n" << std::endl;

  // Run the analog case first and save the grid
  MCWalk analog_walk(grid, _print_grids);
  double analog_FOM = time_walk(analog_walk, 0);
  _walk_data[0].push_back(analog_FOM);
  if (_print_grids) { grid->print(std::cout, _num_samples); }

  // Run all the biased cases
  unsigned int walk_idx = 1;
  MCWalk grid_walk(grid, _print_grids);
  for (size_t i = 1; i < _walk_data.size(); i++) {
    grid_walk.reset();
    grid_walk.set_biased_PDF(_walk_data[i]);
    grid->clear_visits();
    _walk_data[i].push_back(time_walk(grid_walk, i));
    if (_print_grids) { grid->print(std::cout, _num_samples); }
  }
  // Clear visits before returning
  grid->clear_visits();
}

void WalkManager::print_results(std::ofstream & output_file) const {
  output_file << "Entries: " << _walk_data.size() << std::endl;
  output_file << std::fixed;
	output_file << std::showpoint;
	output_file << std::setprecision(5);
  for (const auto& row : _walk_data) {
    // Total the direction PDF parameters to normalize
    double total = std::accumulate(
      row.begin(), row.end()-2, 0.0, std::plus<double>());
    for (int i = 0; i < 8; i++) {
      output_file << std::setw(7) << row[i]/total << " ";
    }
    output_file << std::setw(7) << row[8] << " ";
    output_file << std::setw(7) << row[9] << std::endl;
  }
  output_file << "All Monte Carlo walks ran with " << _num_samples;
  output_file << " samples" << std::endl;
  output_file << "Maximum steps allowed per walk was 100,000" << std::endl;
}