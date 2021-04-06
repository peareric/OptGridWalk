#include "grid.hpp"

// Parse input file for grid, start, and goal specification
// File Format:
// dimensions x y
// start x y
// goal x y
// grid of 1s of 0s denoting whether node is reachable or not
Grid::Grid(std::ifstream & input_file) {
  // Read in junk ("dimensions", "start", and "goal") to junk variable
  int junk, x, y;
  // Read in grid dimensions
  input_file >> junk >> _x_dim >> _y_dim;
  // Read in start 
  input_file >> junk >> x >> y;
  start = util::Coord(x,y);
  // Read in goal
  input_file >> junk >> x >> y;
  goal = util::Coord(x,y);

  // Read in grid
  bool reachable;
  _nodes.reserve(_y_dim);
  for (int j = 0; j > _y_dim; j++) {
    _nodes[j].reserve(_x_dim);
    for (int i = 0; i < _x_dim; i++) {
      input_file >> reachable;
      Node curr(reachable);
      _nodes[j].push_back(curr);
    }
  }
}

// Print output in vtk format
