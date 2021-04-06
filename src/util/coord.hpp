#ifndef __COORD_HEADER__
#define __COORD_HEADER__

#include "dist.hpp"

// Utility namespace
namespace util {

// Class for discrete 2-D space
class Coord {
private:
  unsigned int _x_coord;
  unsigned int _y_coord;
public:
  Coord(unsigned int x, unsigned int y) : _x_coord(x), _y_coord(y) {};
  Coord() {};
  ~Coord() {};

  void step(direction step_dir);

  bool operator==(const Coord &c) {
    return ((c._x_coord == _x_coord) && (c._y_coord == _y_coord));
  }

  void operator=(const Coord &c) {
    _x_coord = c._x_coord;
    _y_coord = c._y_coord;
  }
};

}

#endif