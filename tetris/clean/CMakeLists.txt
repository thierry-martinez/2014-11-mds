cmake_minimum_required(VERSION 2.8)
cmake_policy(VERSION 2.8)
project(tetris)
add_executable(tetris tetris.c window.c grid.c)
add_executable(test_tetris test_tetris.c window.c grid.c)

find_package(GTK2 2.6 REQUIRED gtk)
include_directories(${GTK2_INCLUDE_DIRS})
target_link_libraries(tetris ${GTK2_LIBRARIES})

# set the path to the library folder
#link_directories(/usr/local/lib)

# link the libraries to the executable
target_link_libraries(test_tetris ${GTK2_LIBRARIES})
target_link_libraries (test_tetris cunit)
