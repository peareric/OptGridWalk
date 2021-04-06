#ifndef __NODE_HEADER__
#define __NODE_HEADER__

#include "util/dist.hpp"

#include <vector>

// Class governing a single node of the grid
class Node {
private:
  double _num_visits;
  const bool _is_reachable;
public:
  Node(bool is_reachable) : _num_visits(0), _is_reachable(is_reachable) {};
  ~Node() {};

  void visit() { _num_visits += 1; };
  bool reachable() const { return _is_reachable; };
};

#endif