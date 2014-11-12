
enable_testing()

add_executable(test_tetris test_tetris.c ${CMAKE_SOURCE_DIR}/window.c ${CMAKE_SOURCE_DIR}/grid.c ${CMAKE_SOURCE_DIR}/point.c)
#/${SOURCES})

add_test(test_tetris ${EXECUTABLE_OUTPUT_PATH}/test_tetris)

find_package(GTK2 2.6 REQUIRED gtk)
include_directories(${CMAKE_SOURCE_DIR})
include_directories(${GTK2_INCLUDE_DIRS})
target_link_libraries(test_tetris ${GTK2_LIBRARIES})
target_link_libraries(test_tetris cunit)
