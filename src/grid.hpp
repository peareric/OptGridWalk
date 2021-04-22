#ifndef __GRID_HEADER__
#define __GRID_HEADER__

#include "util/coord.hpp"
#include "util/dist.hpp"

#include <cmath>
#include <fstream>
#include <utility>
#include <vector>

// Class governing the grid of the random walk
// x dimension runs from 0 to _x_dim-1
// y dimension runs from 0 to _y_dim-1
class Grid {
private:
  // Struct governing a single node of the grid
  struct Node {
    Node(bool is_reachable) : _is_reachable(is_reachable) {};
    Node(const Node & other_node)
      : _num_visits(other_node._num_visits),
        _is_reachable(other_node._is_reachable) {};
    Node() : _is_reachable(true) {};
    ~Node() {};

    void operator=(const Node & other_node) {
      _num_visits = other_node._num_visits;
      _is_reachable = other_node._is_reachable;
    }
    // Number of times that the walker was at the coordinate of this node
    unsigned int _num_visits = 0;
    // Whether or not the coordninate of this node is reachable by the walker
    bool _is_reachable;
  };
  // Dimensions of the grid
  unsigned int _x_dim, _y_dim;
  // Start and Goal nodes of the grid
  util::Coord _start, _goal;
  // Nodes of the grid
  std::vector<std::vector<Node>> _nodes;

  // Returns true if node at coordinate is reachable
  bool reachableNode(const util::Coord & coord) const;
public:
  // Ctor from input file
  Grid(std::ifstream & input_file);
  // Deep copy ctor
  Grid(const Grid * other_grid)
    : _x_dim(other_grid->_x_dim), _y_dim(other_grid->_y_dim),
      _start(other_grid->_start), _goal(other_grid->_goal),
      _nodes(other_grid->_nodes) {};
  Grid() {};
  ~Grid() {};

  // Deep copy assignment operator
  void operator=(const Grid * other_grid) {
    _x_dim = other_grid->_x_dim;
    _y_dim = other_grid->_y_dim;
    _start = other_grid->_start;
    _goal = other_grid->_goal;
    _nodes = other_grid->_nodes;
  }

  // Returns the starting coordinate of the walk
  util::Coord get_start() const { return _start; }

  // Returns the goal coordinate of the walk
  util::Coord get_goal() const { return _goal; }

  // Returns a vector of directions that have valid neighboring nodes
  std::vector<util::direction> get_directions(
    const util::Coord & curr_coord) const;

  // Return the number of contiguous valid nodes from coord in dir
  unsigned int get_distance(
    const util::Coord & coord, util::direction dir) const;

  // Tally the number of visits at the current node
  void visit(const util::Coord & coord);

  // Set the number of visits count to zero for all nodes
  void clear_visits();

  // Print the average number of visits to each node per walk
  void print(std::ostream & output_file, double num_walks) const;
};

#endif