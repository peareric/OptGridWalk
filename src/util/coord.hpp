#ifndef __COORD_HEADER__
#define __COORD_HEADER__

// Utility namespace
namespace util {

// Struct for discrete 2-D space
// Contains integers for each dimension and various operators
struct Coord {
  // x and y coordinates
  unsigned int _x_coord = 0;
  unsigned int _y_coord = 0;
  Coord(unsigned int x, unsigned int y) : _x_coord(x), _y_coord(y) {};
  Coord(const Coord & other_coord)
    : _x_coord(other_coord._x_coord), _y_coord(other_coord._y_coord) {};
  Coord() {};
  ~Coord() {};

  bool operator==(const Coord &c) const {
    return ((c._x_coord == _x_coord) && (c._y_coord == _y_coord));
  }

  void operator=(const Coord &c) {
    _x_coord = c._x_coord;
    _y_coord = c._y_coord;
  }

  Coord operator+(const Coord &c) const {
    return Coord(_x_coord+c._x_coord, _y_coord+c._y_coord);
  }

  Coord operator*(const double s) const {
    return Coord(_x_coord*s, _y_coord*s);
  }

  void operator+=(const Coord &c) {
    _x_coord += c._x_coord;
    _y_coord += c._y_coord; 
  }
};

} // end namespace util

#endif