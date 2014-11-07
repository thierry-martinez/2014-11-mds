#include "grid.h"
#include "tetrominos.h"
#include "window.h"

struct coordinates coordinates_of_square(unsigned int square_index) {
  struct tetromino tetromino = tetrominos[current_shape.index];
  int oy = tetromino.coords[square_index][0];
  int ox = tetromino.coords[square_index][1];
  int cy = tetromino.center[0];
  int cx = tetromino.center[1];
  struct coordinates result;
  switch (current_shape.rotation_angle) {
  case ANGLE_0:
    result.column = ox;
    result.row = oy;
    break;
  case ANGLE_90:
    result.column = oy;
    result.row = cx - ox;
    break;
  case ANGLE_180:
    result.column = cx - ox;
    result.row = cy - oy;
    break;
  case ANGLE_270:
    result.column = cy - oy;
    result.row = ox;
    break;
  }
  result.column += current_shape.column_index;
  result.row += current_shape.row_index;
  return result;
}

void fill_current_shape(unsigned int color) {
  unsigned int square_index;
  for (square_index = 0; square_index < NUMBER_OF_SQUARES; square_index++) {
    struct coordinates coordinates = coordinates_of_square(square_index);
    grid[coordinates.row][coordinates.column] = color;
  }
}

void hide_current_shape(void) {
  fill_current_shape(0);
}

void show_current_shape(void) {
  fill_current_shape(current_shape.index + 1);
}

bool is_position_valid() {
  unsigned int square_index;
  for (square_index = 0; square_index < NUMBER_OF_SQUARES; square_index++) {
    struct coordinates coordinates = coordinates_of_square(square_index);
    if (coordinates.column < 0
        || coordinates.row < 0
        || coordinates.column >= NUMBER_OF_COLUMNS
        || coordinates.row >= NUMBER_OF_ROWS
        || grid[coordinates.row][coordinates.column] != 0) {
      return false;
    }
  }
  return true;
}

int move_shape(int x, int y, int o) {
  struct shape old_shape = current_shape;
  hide_current_shape();
  current_shape.column_index += x;
  current_shape.row_index += y;
  current_shape.rotation_angle = (current_shape.rotation_angle + o + 4) % 4;
  int v = is_position_valid();
  if (!v) {
    current_shape = old_shape;
  }
  show_current_shape();
  redraw();
  return v;
}

void move_shape_to_bottom(void) {
  while (move_shape(0, 1, 0));
}

void draw_tetramino(void) {
  next_shape = rand() % NUMBER_OF_TETROMINO_TYPES;
}

int new_shape(void) {
  int v;
  current_shape.index = next_shape;
  draw_tetramino();
  current_shape.column_index = NUMBER_OF_COLUMNS / 2;
  current_shape.row_index = 0;
  current_shape.rotation_angle = 0;
  v = is_position_valid();
  if (v) {
    show_current_shape();
    redraw();
  }
  return v;
}

bool is_row_completed(unsigned int row_index) {
  unsigned int column_index;
  for (column_index = 0; column_index < NUMBER_OF_COLUMNS; column_index++) {
    if (grid[row_index][column_index] == 0) {
      return false;
    }
  }
  return true;
}

void set_row_to_zero(unsigned int row_index) {
  unsigned int column_index;
  for (column_index = 0; column_index < NUMBER_OF_COLUMNS; column_index++) {
    grid[row_index][column_index] = 0;
  }
}

void remove_row(unsigned int removed_row_index) {
  unsigned int row_index, column_index;
  for (row_index = removed_row_index; row_index > 0; row_index--) {
    for (column_index = 0; column_index < NUMBER_OF_COLUMNS; column_index++) {
      grid[row_index][column_index] = grid[row_index-1][column_index];
    }
  }
  set_row_to_zero(0);
}

void detect_lines(void) {
  unsigned int row_index;
  unsigned int count = 0;
  for (row_index = 0; row_index < NUMBER_OF_ROWS; row_index++) {
    if (is_row_completed(row_index)) {
      remove_row(row_index);
      count++;
    }
  }
  if (count > 0) {
    set_score(get_score() + 1 << (2 * (count - 1)));
  }
}

void set_grid_to_zero(void) {
  unsigned int row_index, column_index;
  for (row_index = 0; row_index < NUMBER_OF_ROWS; row_index++) {
    set_row_to_zero(row_index);
  }
}
