# OptGridWalk
This project uses the mlpack machine learning library to optimize a Monte Carlo random walk on an abitrary 2-D grid.

## Grid Specificatoion Format
Grid input files should follow the following convention:
dimensions nodes_in_x_dimension nodes_in_y_dimension
start start_node_x_index start_node_y_index
goal goal_node_x_index goal_node_x_index
[Grid Matrix]
Where the gird matrix has nodes_in_y_dimension rows and nodes_in_x_dimension
columns. The x indices start at 0 on the left and increment to the right, y
indices start at 0 on the top and increment moving down i.e,
(x,y)-->
  |  .
  V    .