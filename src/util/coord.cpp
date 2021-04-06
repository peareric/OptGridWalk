#include "coord.hpp"

#include <stdexcept>

// Utility namespace
namespace util {

void Coord::step(direction step_dir) {
  switch (step_dir) {
    case north:
      _y_coord += 1;
      break;
    case north_east:
      _x_coord += 1;
      _y_coord += 1;
      break;
    case east:
      _x_coord += 1;
      break;
    case south_east:
      _x_coord += 1;
      _y_coord -= 1;
      break;
    case south:
      _y_coord -= 1;
      break;
    case south_west:
      _x_coord -= 1;
      _y_coord -= 1;
      break;
    case west:
      _x_coord -= 1;
      break;
    case north_west:
      _x_coord -= 1;
      _y_coord += 1;
      break;
    default:
      throw std::runtime_error("Unknown direction to step in");
  }
}

} // end namespace util