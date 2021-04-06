#include "dist.hpp"

#include <cmath>
#include <numeric>
#include <stdexcept>

// Utility namespace
namespace util {

// truncated exponential PDF given by:
// lambda*exp(-lambda*(x-a))/(1-exp(-lambda*(b-a))); a<=x<=b
double PDF::sample(
    const double a, const double b, const double lambda,
    dist_type type) const {
  return a - std::log(1-_rng_ptr->sample()*(1 - std::exp(-lambda*(b-a))))/lambda;
}
double PDF::evaluate(
    const double a, const double b, const double lambda,
    const double point,
    dist_type type = dist_type::truncated_exponential) const {
  if (point < a or point > b) {
    throw std::runtime_error(
      "Truncated exponential evaluated outside of support.");
  }
  return lambda*exp(-lambda*(point-a))/(1-exp(-lambda*(b-a)));
}

// Categorical PDF
// Index of probability is returned and evaluted
// Probabilities is assumed to be strictly positive
double PDF::sample(
    const std::vector<double> &probabilities, dist_type type) const {
  double stop = _rng_ptr->sample();
  double total = std::accumulate(probabilities.begin(), probabilities.end(), 0);
  for (size_t i = 0; i < probabilities.size(); i++) {
    stop -= probabilities[i]/total;
    if (stop <= 0) {
      return (double) i;
    }
  }
  // Maybe exit loop on floating point roundoff error
  return (double) probabilities.size()-1;
}
double PDF::evaluate(
    const std::vector<double> &probabilities, const double idx,
    dist_type type) const {
  return probabilities.at((int)idx);
}

} // end namespace util