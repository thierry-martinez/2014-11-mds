cmake_minimum_required(VERSION 2.8)
cmake_policy(VERSION 2.8)
project(tetris)

SET( CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -Wall -Wextra" )

set(SOURCES window.c grid.c point.c)

add_executable(tetris tetris.c ${SOURCES})

find_package(GTK2 2.6 REQUIRED gtk)
include_directories(${GTK2_INCLUDE_DIRS})
target_link_libraries(tetris ${GTK2_LIBRARIES})

add_subdirectory(test)

