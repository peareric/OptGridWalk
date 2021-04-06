#ifndef __WALKER_HEADER__
#define __WALKER_HEADER__

#include "util/coord.hpp"

#include <algorithm>

// Class governing the object walking from start to finish
class Walker {
private:
  double _weight;
  double _lambda;
  std::vector<double> _direction_probabilities;
  util::Coord _position;
public: 
  Walker() : _weight(1.0), _lambda(1.0) {
  std::fill(
    _direction_probabilities.begin(), _direction_probabilities.end(), 0.125);
  }
  ~Walker() {};

  void set_direction_PDF(const std::vector<double> &probabilities) {
    _direction_probabilities = probabilities;
  }
  void set_length_PDF(const double lambda) { _lambda = lambda; }

  void set_position(util::Coord start) { _position = start; }

  const std::vector<double> & get_direction_PDF() const { 
    return _direction_probabilities; 
  }
  const double get_length_PDF() const { return _lambda; }
};

#endif