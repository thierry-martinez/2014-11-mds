add_executable(tetris tetris.c window.c grid.c)

find_package(GTK2 2.6 REQUIRED gtk)
include_directories(${GTK2_INCLUDE_DIRS})
target_link_libraries(tetris ${GTK2_LIBRARIES})
