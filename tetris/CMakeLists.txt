cmake_minimum_required(VERSION 2.8)
cmake_policy(VERSION 2.8)
project(tetris)
add_executable(tetris tetris.c)

find_package(GTK2 2.6 REQUIRED gtk)
include_directories(${GTK2_INCLUDE_DIRS})
target_link_libraries(tetris ${GTK2_LIBRARIES})
