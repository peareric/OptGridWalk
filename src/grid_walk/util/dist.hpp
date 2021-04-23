#ifndef __PDF_HEADER__
#define __PDF_HEADER__

#include "coord.hpp"
#include "rand.hpp"

#include <utility>
#include <vector>

// Utility namespace
namespace util {

// Enumerated list of all distribution types
enum dist_type {truncated_exponential, categorical, guassian, uniform};

// Enumerated list of all directions 
// Note standard guarantees that the underlaying enums range in order 0-7
enum direction {north, north_east, east, south_east,
                south, south_west, west, north_west};

// Vector of direction enum for safe iteration
static const std::vector<direction> all_directions = {
  direction::north, direction::north_east,
  direction::east, direction::south_east,
  direction::south, direction::south_west,
  direction::west, direction::north_west};

// Returns coordinate with x, y increments corresponding to a given direction
util::Coord to_increment(direction dir);

// Class for all probability density distributions
class PDF {
private:
  // Single RNG used throughout for consistency
  mutable RNG _rng;

public:
  PDF(RNG rng) : _rng(rng) {};
  ~PDF() {};

  // Returns the rng to the default state
  void reset_rng() { _rng.set_seed(); }

  // PDF is deduced by operator overloading or type enum
  // sample: samples a random point from the PDF
  // evalute: evaulates the PDF at a given point

  // Truncated Exponential PDF
  unsigned int sample(
    const unsigned int b, const double lambda,
    dist_type type = dist_type::truncated_exponential) const;
  double evaluate(
    const unsigned int b, const double lambda,
    const unsigned int point,
    dist_type type = dist_type::truncated_exponential) const;

  // Categorical PDF
  // Index of probability is returned and evaluted
  unsigned int sample(
    const std::vector<double> &probabilities,
    dist_type type = dist_type::categorical) const;
  double evaluate(
    const std::vector<double> &probabilities, const unsigned int idx,
    dist_type type = dist_type::categorical) const;

  // Independent Gaussian PDFs
  // std_2 is applied to last element, std_1 is applied to the rest
  std::vector<double> sample(
      const std::vector<double> & means, const double std_1,
      const double std_2, dist_type type = dist_type::guassian) const;

  // Sample PRNG [0,1]
  double sample(dist_type type = dist_type::uniform) {
    return _rng.sample();
  }
  double evaluate(dist_type type = dist_type::uniform) {
    return 1.0;
  }
};

} // end namespace util

#endif