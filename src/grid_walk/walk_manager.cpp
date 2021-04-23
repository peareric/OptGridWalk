#include "walk_manager.hpp"

#include "util/rand.hpp"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>

// Reads the simulation specifications,
// either training data generation or simulated annealing
//
// File format for training data generation:
// Entries [number_of_PMF_sets]
// Samples [number_of_samples_per_walk]
// Print Spatial Distributions [0/1]
// [direction pmf values] [distance pmf value]
//                       .
//                       .
//                       .
// Direction information is stored in the order:
// north, north_east, east, south_east, south, south_west, west, north_west
//
// File format for simulated annealing:
// Optimization [number_of_function_evaluations]
// Samples [number_of_samples_per_walk]
// Print Spatial Distributions [0/1]
WalkManager::WalkManager(std::ifstream & input_file)
    : _prob_distributions(util::RNG()) {
  // Read in simulation specifications
  std::string run_type;
  std::string junk;
  int num;
  input_file >> run_type >> num;
  input_file >> junk >> _num_samples;
  input_file >> junk >> junk >> junk >> _print_grids;

  // Add analog simulation to list regardless of simulation mode
  _walk_data.push_back(std::vector<double>(
    {0.125,0.125,0.125,0.125,0.125,0.125,0.125,0.125,1.0}));
  
  // Setup to simulate all passed PMF parameters
  if (run_type == "Entries" || run_type == "entries") {
    _optimize=false;
    _walk_data.reserve(num+1);
    for (int i = 0; i < num; i++) {
      double val;
      std::vector<double> row_data;
      for (int j = 0; j < 9; j++) {
        input_file >> val;
        row_data.push_back(val);
      }
      _walk_data.push_back(row_data);
    }
  }
  // Setup to perform optimization
  else if (run_type == "Optimization" | run_type == "optimization") {
    _optimize=true;
    _num_evals=num;
  }
  else {
    throw std::runtime_error(
      "Input file parameter "+run_type+" not recognized");
  }
}

// Perform walk for the passed mc_walk object with timing and printing
// return the mean number of steps 
double WalkManager::time_walk(MCWalk & walk, int i) const {
  std::cout << "Starting walk " << i << std::endl;
  auto start = std::chrono::steady_clock::now();
  double mean = walk.walk_grid(_num_samples);
  auto end = std::chrono::steady_clock::now();
  std::cout << "Random walk complete" << std::endl;
  std::cout << "Clock time: "
            << std::chrono::duration_cast<std::chrono::seconds>(
              end - start).count() << " sec" << std::endl;
  walk.print_results();
  return mean;
}

// Simulate all passed PMF parameters
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
  MCWalk grid_walk(grid, _print_grids);
  for (size_t i = 1; i < _walk_data.size(); i++) {
    grid_walk.reset();
    grid_walk.set_biased_PMF(_walk_data[i]);
    grid->clear_visits();
    _walk_data[i].push_back(time_walk(grid_walk, i));
    if (_print_grids) { grid->print(std::cout, _num_samples); }
  }
  // Clear visits before returning
  grid->clear_visits();
}

// Perform simulated annealing starting with analog case
void WalkManager::simulate_annealing(Grid * grid) {
  std::cout << "Running " << _num_evals << " random walks with ";
  std::cout << _num_samples << " samples each" << std::endl;
  std::cout << "Walk 0 is analog walk\n" << std::endl;

  // Run the analog case first and save the grid
  MCWalk analog_walk(grid, _print_grids);
  double analog_mean = time_walk(analog_walk, 0);
  _walk_data[0].push_back(analog_mean);
  if (_print_grids) { grid->print(std::cout, _num_samples); }

  // Save the index of the currently most optimal parameters and value
  int _min_idx = 0;
  // Simulate annealing
  MCWalk grid_walk(grid, _print_grids);
  for (int i = 1; i < _num_evals; i++) {
    // Logarithmic cooling T_0 = 0.1
    double temp = -0.1*std::log(i/_num_evals);
    std::vector<double> candidate = _prob_distributions.sample(
      std::vector<double>(_walk_data.back().begin(), _walk_data.back().end()-1),
      temp, 10.0*temp, util::dist_type::guassian);
    // Don't allow negative probabilities
    for (int i = 0; i < 8; i++) { if (candidate[i] < 0) { candidate[i] = 0; } }

    // Evaluate candidate
    grid_walk.reset();
    grid_walk.set_biased_PMF(candidate);
    grid->clear_visits();
    candidate.push_back(time_walk(grid_walk, i));
    if (_print_grids) { grid->print(std::cout, _num_samples); }

    // Accept or reject candidate
    if (_prob_distributions.sample(util::dist_type::uniform) <=
        std::min(
          1.0, std::exp(-(candidate.back()-_walk_data.back().back())/temp))) {
      _walk_data.push_back(candidate);
      // Save new global min if found
      if (candidate.back() < _walk_data[_min_idx].back()) {
        _min_idx = _walk_data.size()-1;
      }
    }
  }

  std::cout << "\n\nOptimization Complete!" << std::endl;
  std::cout << "Analog Case" << std::endl;
  MCWalk final_walk(grid, true);
  final_walk.print_walker();
  grid->clear_visits();
  analog_mean = time_walk(final_walk, _num_evals+1);
  grid->print(std::cout, _num_samples);
  std::cout << "Optimized Case" << std::endl;
  grid->clear_visits();
  final_walk.set_biased_PMF(std::vector<double>(
    _walk_data[_min_idx].begin(), _walk_data[_min_idx].end()-1));
  final_walk.print_walker();
  double opt_mean = time_walk(final_walk, _num_evals+2);
  grid->print(std::cout, _num_samples);

  // Clear visits before returning
  grid->clear_visits();
}

void WalkManager::execute(Grid * grid) {
  if (_optimize) {
    simulate_annealing(grid);
  }
  else {
    run_all_cases(grid);
  }
}

void WalkManager::print_results(std::ofstream & output_file) const {
  output_file << "Entries: " << _walk_data.size() << std::endl;
  output_file << std::fixed;
	output_file << std::showpoint;
	output_file << std::setprecision(5);
  for (const auto& row : _walk_data) {
    // Total the direction PMF parameters to normalize
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