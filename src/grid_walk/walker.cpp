#include "walker.hpp"

#include "util/dist.hpp"

#include <cmath>
#include <iomanip>
#include <iostream>

// Samples a direction to walk in randomly
util::direction Walker::sample_dir(const Grid * grid) {
  // Get the possible directions from the grid
  std::vector<util::direction> possible_dirs = 
    grid->get_directions(_position);

  // If all direction are possible, sample the direction to take
  unsigned int dir_idx;
  std::vector<double> possible_dir_probs;
  if (possible_dirs.size() == util::all_directions.size()) {
    possible_dir_probs = _direction_probabilities;
    dir_idx = _prob_distributions.sample(
      possible_dir_probs, util::dist_type::categorical);
  }
  // Otherwise construct a vector of only possible direction probabilies
  // and sample direction from it
  else {
    for (const auto dir : possible_dirs) {
      possible_dir_probs.push_back(_direction_probabilities[dir]);
    }
    dir_idx = _prob_distributions.sample(
      possible_dir_probs, util::dist_type::categorical);
  }

  // Adjust the weight to match analog case, analog direction is equiprobable
  // [no longer used, importance sampling commented out]
  if (_biased_walk) {
    // analog prob is 1/num_dirs
    double bias_ratio = 1.0 / (_prob_distributions.evaluate(
        possible_dir_probs, dir_idx, util::dist_type::categorical) *
        possible_dir_probs.size());
    _weight *= bias_ratio;
  }

  // Sample the direction from the vector of possible directions
  return possible_dirs[dir_idx];
}

// Returns the distance to travel
unsigned int Walker::sample_dist(const Grid * grid, const util::direction dir){
  // Get the total valid distance
  unsigned int total = grid->get_distance(_position, dir);
  // Sample the distance to travel along the total
  unsigned int travel_dist = _prob_distributions.sample(
    total, _lambda, util::dist_type::truncated_exponential);

  // Adjust the weight to match analog case, analog lambda is 1
  // [no longer used, importance sampling commented out]
  if (_biased_walk) {
    double bias_ratio = _prob_distributions.evaluate(
        total, 1.0, travel_dist,
        util::dist_type::truncated_exponential) /
      _prob_distributions.evaluate(
        total, _lambda, travel_dist,
        util::dist_type::truncated_exponential);
    _weight *= bias_ratio;
  }

  return travel_dist;
}

void Walker::set_biased_PMF(const std::vector<double> &probabilities) {
  // Importance sampling commented out
  // _biased_walk = true;
  _direction_probabilities = std::vector<double>(
    probabilities.begin(), probabilities.end()-1);
  _lambda = probabilities.back();
}

// Randomly samples the next grid node of the walker
void Walker::step(const Grid * grid) {
  // Sample the direction to move in
  auto dir = sample_dir(grid);
  // Sample the number of node to traverse in that direction
  auto dist = sample_dist(grid, dir);
  // Move the walker to the new node
  _position += util::to_increment(dir)*dist;
}

void Walker::print_PMF_paramters() const {
  std::cout << std::fixed;
	std::cout << std::showpoint;
	std::cout << std::setprecision(4);
  double total = std::accumulate(
    _direction_probabilities.begin(), _direction_probabilities.end(), 0.0,
    std::plus<double>());
  std::cout << std::setw(7) << "N   " << std::setw(7) << "NW  ";
  std::cout << std::setw(7) << "W   " << std::setw(7) << "SW  ";
  std::cout << std::setw(7) << "S   " << std::setw(7) << "SE  ";
  std::cout << std::setw(7) << "E   " << std::setw(7) << "SE  ";
  std::cout << std::setw(7) << "lambda " << std::endl;
  for (const auto& dir : _direction_probabilities) {
    std::cout << std::setw(5) << dir/total << " ";
  }
  std::cout << std::setw(5) << _lambda << std::endl;
}

