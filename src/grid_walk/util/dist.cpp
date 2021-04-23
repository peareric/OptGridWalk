#include "dist.hpp"

#include <cmath>
#include <numeric>
#include <stdexcept>

static const double pi = 3.14159265358979323846;

// Utility namespace
namespace util {

// Recall input format
// (x,y)-->
//  |  .
//  V    .
// Returns coordinate pair corresponding to the x and y increments of the
// direction
util::Coord to_increment(direction dir) {
  unsigned int x,y;
  switch (dir) {
    case north:
      x =  0;
      y =  -1;
      break;
    case north_east:
      x =  1;
      y =  -1;
      break;
    case east:
      x =  1;
      y =  0;
      break;
    case south_east:
      x =  1;
      y =  1;
      break;
    case south:
      x =  0;
      y =  1;
      break;
    case south_west:
      x = -1;
      y =  1;
      break;
    case west:
      x = -1;
      y =  0;
      break;
    case north_west:
      x = -1;
      y = -1;
      break;
    default:
      throw std::runtime_error("Unknown direction to step in");
  }
  return util::Coord(x,y);
}

// continuous truncated exponential PDF given by:
// lambda*exp(-lambda*(x-a))/(1-exp(-lambda*(b-a))); 0<=x<=b
unsigned int PDF::sample(
    const unsigned int b, const double lambda, dist_type type) const {
  return std::ceil(-std::log(1-_rng.sample()*(1 - std::exp(-lambda*b)))/lambda);
}
double PDF::evaluate(
    const unsigned int b, const double lambda, const unsigned int point,
    dist_type type) const {
  if (point > b) {
    throw std::runtime_error(
      "Truncated exponential evaluated outside of support.");
  }
  // Get CDF at previous discrete point
  double cdf_prev = (point == 0) ? 0 :
    (1-std::exp(-lambda*(point-1))) / (1-std::exp(-lambda*b));
  // Get the CDF at the current point
  double cdf = (1-std::exp(-lambda*point)) / (1-std::exp(-lambda*b));
  return cdf-cdf_prev;
}

// Categorical PDF
// Index of probability is returned and evaluted
// Probabilities is assumed to be strictly positive
unsigned int PDF::sample(
    const std::vector<double> &probabilities, dist_type type) const {
  double stop = _rng.sample();
  double total = std::accumulate(
    probabilities.begin(), probabilities.end(), 0.0, std::plus<double>());
  for (size_t i = 0; i < probabilities.size(); i++) {
    stop -= probabilities[i]/total;
    if (stop <= 0) {
      return i;
    }
  }
  // Maybe exit loop on floating point roundoff error
  return probabilities.size()-1;
}
double PDF::evaluate(
    const std::vector<double> &probabilities, const unsigned int idx,
    dist_type type) const {
  double total = std::accumulate(
    probabilities.begin(), probabilities.end(), 0.0, std::plus<double>());
  return probabilities.at(idx)/total;
}

// N-dimensional independent Gaussisan pdf 
std::vector<double> PDF::sample(
    const std::vector<double> & means, const double stdev,
    dist_type type) const {
  std::vector<double> samples;
  for (const double mean : means) {
    samples.push_back(
      mean + stdev*std::sqrt(-2*std::log(_rng.sample()))
                  *std::cos(2*pi*_rng.sample()));
  }
  return std::move(samples);
}

} // end namespace util