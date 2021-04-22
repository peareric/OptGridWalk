#include "walk_manager.hpp"

#include <chrono>
#include <iostream>
#include <string>

// Reads in the biased PDFs to simulate and saves them to the results vector
// File format is
// Entries N
// [direction pdf values] [distance pdf value]
// Where direction information is stored in the order:
// north, north_east, east, south_east, south, south_west, west, north_west
WalkManager::WalkManager(std::ifstream & input_file, double num_samples) 
    : _num_samples(num_samples) {
  std::string junk;
  int num_entries;
  input_file >> junk >> num_entries;
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
  std::cout << _num_samples << " samples each\n" << std::endl;

  // Run the analog case first and save the grid
  MCWalk analog_walk(grid, true);
  double analog_FOM = time_walk(analog_walk, 0);
  _analog_grid = *grid;
  _walk_data[0].push_back(analog_FOM);

  // Run all the biased cases
  unsigned int walk_idx = 1;
  MCWalk grid_walk(grid);
  for (size_t i = 1; i < _walk_data.size(); i++) {
    grid_walk.reset();
    grid_walk.set_biased_PDF(_walk_data[i]);
    _walk_data[i].push_back(time_walk(grid_walk, i));
  }

  // Clear visits before returning
  grid->clear_visits();
}