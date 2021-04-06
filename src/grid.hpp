#ifndef __GRID_HEADER__
#define __GRID_HEADER__

#include "util/coord.hpp"
#include "util/dist.hpp"
#include "node.hpp"

#include <fstream>
#include <utility>
#include <vector>

// Class governing the grid of the random walk
// x dimension runs from 0 to _x_dim
// y dimension runs from 0 to _y_dim
class Grid {
private:
  // Dimensions of the grid
  int _x_dim, _y_dim;
  // Start and Goal nodes of the grid
  util::Coord start, goal;
  // Nodes of the grid
  std::vector<std::vector<Node>> _nodes;
public:
  Grid(std::ifstream & input_file);
  ~Grid() {};

  util::Coord get_start() const { return start; }

  void print(std::ofstream & output_file, double num_walks);
};

#endif