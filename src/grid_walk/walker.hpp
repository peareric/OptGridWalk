#ifndef __WALKER_HEADER__
#define __WALKER_HEADER__

#include "util/grid.hpp"
#include "util/coord.hpp"
#include "util/dist.hpp"

#include <algorithm>

// Class governing the object walking from start to finish
class Walker {
private:
  // Statistical weight of the particle 
  // [no longer used, importance sampling commented out]
  double _weight = 1.0;
  // Parameter of dicrete trunacted exponential govering distance
  double _lambda = 1.0;
  // Whether importance sampling is in play
  // [no longer used, importance sampling commented out]
  const bool _biased_walk = false;
  // Vector of relative probabilies of each direction,
  // order is clockwise starting at north
  std::vector<double> _direction_probabilities;
  // Current position
  util::Coord _position;
  // Probability denisty function object
  util::PDF _prob_distributions;

  // Returns a randomly sampled direction from all possible directions
  // according to the probability of each
  util::direction sample_dir(const Grid * grid);

  // Return a ramdonly sampled distance to travel in a given direction for all
  // all possible distance according to the truncated exponential
  unsigned int sample_dist(const Grid * grid, const util::direction dir);

public: 
  Walker() : _prob_distributions(util::RNG()) {
  _direction_probabilities.resize(8);
  std::fill(
    _direction_probabilities.begin(), _direction_probabilities.end(), 0.125);
  }
  ~Walker() {};

  // Sets the weight to one and resets the RNG to the initial state
  void reset() {
    _weight = 1.0;
    _prob_distributions.reset_rng();
  }

  // Reset the weight of the particle to one
  // [no longer used, importance sampling commented out]
  void reset_weight() { _weight = 1.0; }

  // Set the PMF to biased values
  // Probabilities are assumed to be set in the following order:
  // [direction PMF] [distance PMF]
  // Direction parameters are passed in the order:
  // north, north_east, east, south_east,
  // south, south_west, west, north_west
  void set_biased_PMF(const std::vector<double> &probabilities);

  // Modify the position
  void set_position(util::Coord start) { _position = start; }

  // Return the weight of the walker
  // [no longer used, importance sampling commented out]
  const double get_weight() const { return _weight; }

  // Return true if walker position is passed coordinate
  bool at_coordinate(const util::Coord & p) {return _position == p; }

  // Randomly sample a direction and distance and adjust the position and
  // weight accordingly
  void step(const Grid * grid);

  // Call visit on the grid at the current position
  void visit_grid(Grid * grid) const { grid->visit(_position); }

  // Print the parameters of the direction and distance PMF to standard out
  void print_PMF_paramters() const;
};

#endif