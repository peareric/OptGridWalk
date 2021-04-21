#include "grid.hpp"

#include <iomanip>
#include <iostream>
#include <stdexcept>

bool Grid::validNode(const util::Coord & coord) const {
  return coord._x_coord > -1 && coord._x_coord < _x_dim &&
         coord._y_coord > -1 && coord._y_coord < _y_dim &&
         _nodes[coord._y_coord][coord._x_coord]._is_reachable;
}

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

std::vector<util::direction> Grid::get_directions(
      const util::Coord & curr_coord) const {
  std::vector<util::direction> possible_dirs;
  for (const auto dir : util::all_directions) {
    auto incr = util::to_increment(dir);
    if (validNode(curr_coord+incr)) {
      possible_dirs.push_back(dir);
    }
  }
  return std::move(possible_dirs);
}

unsigned int Grid::get_distance(
    const util::Coord & coord, util::direction dir) const {
  unsigned int num_nodes = 0;
  auto incr = util::to_increment(dir);
  auto runner = coord + incr;
  while(validNode(runner)) { 
    runner += incr;
    ++num_nodes;
  }
  return num_nodes;
}

void Grid::visit(const util::Coord & coord) {
  if (validNode(coord)) {
    _nodes[coord._y_coord][coord._x_coord]._num_visits += 1;
  }
  else {
    throw std::runtime_error("Attempt to visit invalid node!");
  }
}

void Grid::print(std::ostream & output_file, double num_walks) {
  std::cout << std::fixed;
	std::cout << std::showpoint;
	std::cout << std::setprecision(5);
  for (int y = 0; y < _y_dim; y++) {
    for (int x = 0; x < _x_dim; x++) {
      std::cout << std::setw(8) << _nodes[y][x]._num_visits / num_walks << " ";
    }
    std::cout << std::endl;
  }
}