#ifndef TETRIS_GRID_H
#define TETRIS_GRID_H

#include <stdbool.h>

#define NUMBER_OF_ROWS 16
#define NUMBER_OF_COLUMNS 16

static unsigned int const SQUARE_SIDE_LENGTH = 24;

typedef enum {
  Normal = 0,
  Paused,
  Finished
} GameState;

struct game {
  unsigned int score;
  unsigned int grid[NUMBER_OF_ROWS][NUMBER_OF_COLUMNS];
  GameState state;
} game;

/* Status */

GameState get_state(void);
void set_state(unsigned int new_state);

struct coordinates {
  int column, row;
};

struct coordinates coordinates_of_square(unsigned int square_index);
void fill_current_shape(unsigned int color);
void hide_current_shape(void);
void show_current_shape(void);
bool is_position_valid(void);
int move_shape(int x, int y, int o);
unsigned int random_tetromino_type(void);
int new_shape(void);
bool is_row_completed(unsigned int row_index);
void set_row_to_zero(unsigned int row_index);
void remove_row(unsigned int removed_row_index);
void detect_lines(void);
void set_grid_to_zero(void);

void move_shape_to_bottom(void);

#endif
