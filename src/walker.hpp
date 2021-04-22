#ifndef __WALKER_HEADER__
#define __WALKER_HEADER__

#include "grid.hpp"
#include "util/coord.hpp"
#include "util/dist.hpp"

#include <algorithm>

// Class governing the object walking from start to finish
class Walker {
private:
  double _weight = 1.0;
  double _lambda = 1.0;
  bool _biased_walk = false;
  std::vector<double> _direction_probabilities;
  util::Coord _position;
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
  void reset_weight() { _weight = 1.0; }

  // Set the PDF to biased values
  // Probabilities are assumed to be set in the following order:
  // [direction PDF] [distance PDF]
  // Direction parameters are passed in the order:
  // north, north_east, east, south_east,
  // south, south_west, west, north_west
  void set_biased_PDF(const std::vector<double> &probabilities);

  // Modify the position
  void set_position(util::Coord start) { _position = start; }

  // Return the weight of the walker 
  const double get_weight() const { return _weight; }

  // Return true if walker position is passed coordinate
  bool at_coordinate(const util::Coord & p) {return _position == p; }

  // Randomly sample a direction and distance and adjust the position and
  // weight accordingly
  void step(const Grid * grid);

  // Call visit on the grid at the current position
  void visit_grid(Grid * grid) const { grid->visit(_position); }
};

#endif