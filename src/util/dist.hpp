#ifndef __PDF_HEADER__
#define __PDF_HEADER__

#include "coord.hpp"
#include "rand.hpp"

#include <utility>
#include <vector>

// Utility namespace
namespace util {

// Enumerated list of all distribution types
enum dist_type {truncated_exponential, categorical};

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

  // PDF is deduced by operator overloading or type enum
  // sample: samples a random point from the PDF
  // evalute: evaulates the PDF at a given point

  // Truncated Exponential PDF
  double sample(
    const double a, const double b, const double lambda,
    dist_type type = dist_type::truncated_exponential) const;
  double evaluate(
    const double a, const double b, const double lambda,
    const double point,
    dist_type type = dist_type::truncated_exponential) const;

  // Categorical PDF
  // Index of probability is returned and evaluted
  double sample(
    const std::vector<double> &probabilities,
    dist_type type = dist_type::categorical) const;
  double evaluate(
    const std::vector<double> &probabilities, const double idx,
    dist_type type = dist_type::categorical) const;
};

} // end namespace util

#endif