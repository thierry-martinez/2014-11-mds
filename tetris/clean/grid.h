#ifndef TETRIS_GRID_H
#define TETRIS_GRID_H

#include <stdbool.h>

#define NUMBER_OF_ROWS 16
#define NUMBER_OF_COLUMNS 16

static unsigned int const SQUARE_SIDE_LENGTH = 16;
unsigned int grid[NUMBER_OF_ROWS][NUMBER_OF_COLUMNS];

int column_index_of_square(unsigned int square_index);
int row_index_of_square(unsigned int square_index);
void fill_current_shape(unsigned int color);
bool valid_position();
int move_shape(int x, int y, int o);
void draw_tetramino();
int new_shape();
bool complete_row(unsigned int row_index);
void set_row_to_zero(unsigned int row_index);
void remove_row(unsigned int removed_row_index);
void detect_lines();
void set_grid_to_zero();

void move_shape_to_bottom();

#endif
