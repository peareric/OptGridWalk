cmake_minimum_required(VERSION 3.12.4)
project(GridWalkOpt)
set(CMAKE_CXX_STANDARD 17)
if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "DEBUG")
endif ()
file(GLOB_RECURSE GRID_WALK_CPP_FILES "src/grid_walk/*.cpp")
add_executable(gridwalk ${GRID_WALK_CPP_FILES})
  
