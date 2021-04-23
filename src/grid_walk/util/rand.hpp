#ifndef __RAND_HEADER__
#define __RAND_HEADER__

#include <random>

// Utility namespace
namespace util {

// Simple pseudorandom number generator
class RNG {
private:
  std::mt19937_64 _engine;
  std::uniform_int_distribution<uint64_t> _int_dist;

public:
  RNG(double seed = 16180339) : _engine(seed) {};
  ~RNG() {};

  // Return a number uniformly distributed on [0,1]
  double sample() { return (double) _int_dist(_engine)/UINT64_MAX; }

  // Sets the seed of the engine and resets the distribution
  void set_seed(double seed = 16180339) {
    _engine = std::mt19937_64(seed);
    _int_dist.reset();
  }
};

} // end namespace util

#endif