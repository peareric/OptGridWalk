#ifndef __PDF_HEADER__
#define __PDF_HEADER__

#include "rand.hpp"

#include <vector>

// Utility namespace
namespace util {

// Enumerated list of all distribution types
const enum dist_type {truncated_exponential, categorical_direction};

// Enumerated list of all directions 
const enum direction {north, north_east, east, south_east,
                      south, south_west, west, north_west};
// Vector of direction enum for safe iteration
static const std::vector<direction> all_directions = {
  direction::north, direction::north_east,
  direction::east, direction::south_east,
  direction::south, direction::south_west,
  direction::west, direction::north_west};

// Class for all probability density distributions, PDF is deduced by operator
// overloading or type enum
// sample: samples a random point from the PDF
// evalute: evaulates the PDF at a given point
class PDF {
private:
  RNG * const _rng_ptr;

public:
  PDF(RNG * rng_ptr) : _rng_ptr(rng_ptr) {};
  ~PDF() {};

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
    dist_type type = dist_type::categorical_direction) const;
  double evaluate(
    const std::vector<double> &probabilities, const double idx,
    dist_type type = dist_type::categorical_direction) const;
};

} // end namespace util

#endif