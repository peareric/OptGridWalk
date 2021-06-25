# Monte Carlo Grid Walk Optimization 
This project aims to use the mlpack machine learning library to optimize a 
Monte Carlo random walk on an abitrary 2-D grid. 

## Method of Optimization First, a set of training data is produced
by repeated Monte Carlo calculations. These calulations are performed by the
`gridwalk` executable on a specified grid for the "analog" case i.e., all
possible directions are travel equiprobable and distances of travel distributed
by a truncated exponential with parameter 1, as well as for cases with other
probability density function parameters. Grids are specified in the grid input
file and probability denisity function parameters are specified in the walk
parameters input file. The `gridwalk` execuatble is called and passed both the
grid input file and the walk parameters input file as `./gridwalk grid_file
walk_params_file`. 

## Grid Specification Format Grid input files should follow
the following convention: 
dimensions [nodes_in_x_dimension] [nodes_in_y_dimension] 
start [start_node_x_index] [start_node_y_index] 
goal [goal_node_x_index] [goal_node_x_index] 
[Grid Matrix] 
Where the gird matrix has nodes_in_y_dimension rows and 
nodes_in_x_dimension columns. The x indices start at 0 on the left and 
increment to the right, y indices start at 0 on the top and increment moving 
down i.e,   
<pre>
(x,y)-->  
  |  .  
  V    .  
</pre>
  
## Walk Parameter Specification Format 
Walk parameters input files should follow the following convention: 
entries [N1] 
samples [N2] 
print spatial distributions [0/1]
[direction pdf values] [distance pdf value] 
Where N1 is the number of biased PDF entries, N2 is the number of samples to 
perform per random walk, [0/1] indicates whether to print the spatial 
distribution of each set of walk parameters, and direction information is 
passed in the order:
north, north_east, east, south_east, south, south_west, west, north_west
