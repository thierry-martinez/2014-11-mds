#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "tetrominos.h"
#include "grid.h"
#include "window.h"





int column_index_of_square(unsigned int square_index) {
  int oy = tetrominos[current_shape.index].coords[square_index][0];
  int ox = tetrominos[current_shape.index].coords[square_index][1];
  int cy = tetrominos[current_shape.index].center[0];
  int cx = tetrominos[current_shape.index].center[1];
  int x;
  if (current_shape.rotation_angle == 0) {
    x = ox;
  }
  if (current_shape.rotation_angle == 1) {
    x = oy;
  }
  if (current_shape.rotation_angle == 2) {
    x = cx - ox;
  }
  if (current_shape.rotation_angle == 3) {
    x = cy - oy;
  }
  x += current_shape.column_index;
}

int row_index_of_square(unsigned int square_index) {
  int oy = tetrominos[current_shape.index].coords[square_index][0];
  int ox = tetrominos[current_shape.index].coords[square_index][1];
  int cy = tetrominos[current_shape.index].center[0];
  int cx = tetrominos[current_shape.index].center[1];
  int y;
  if (current_shape.rotation_angle == 0) {
    y = oy;
  }
  if (current_shape.rotation_angle == 1) {
    y = cx - ox;
  }
  if (current_shape.rotation_angle == 2) {
    y = cy - oy;
  }
  if (current_shape.rotation_angle == 3) {
    y = ox;
  }
  y += current_shape.row_index;
}

void fill_current_shape(unsigned int color) {
  unsigned int square_index;
  for (square_index = 0; square_index < NUMBER_OF_SQUARES; square_index++) {
    int row_index = row_index_of_square(square_index);
    int column_index = column_index_of_square(square_index);
    grid[row_index][column_index] = color;
  }
}

bool valid_position() {
  unsigned int square_index;
  for (square_index = 0; square_index < NUMBER_OF_SQUARES; square_index++) {
    int row_index = row_index_of_square(square_index);
    int column_index = column_index_of_square(square_index);
    if (!(column_index >= 0 && row_index >= 0 && column_index < NUMBER_OF_COLUMNS && row_index < NUMBER_OF_ROWS && grid[row_index][column_index] == 0)) {
      return false;
    }
  }
  return true;
}

int move_shape(int x, int y, int o) {
  struct shape old_shape = current_shape;
  fill_current_shape(0);
  current_shape.column_index += x;
  current_shape.row_index += y;
  current_shape.rotation_angle += o;
  if (current_shape.rotation_angle > 3) {
    current_shape.rotation_angle -= 4;
  }
  if (current_shape.rotation_angle < 0) {
    current_shape.rotation_angle += 4;
  }
  int v = valid_position();
  if (!v) {
    current_shape = old_shape;
  }
  fill_current_shape(current_shape.index + 1);
  gtk_widget_queue_draw(window);
  return v;
}

void draw_tetramino() {
  next_shape = rand() % NUMBER_OF_TETROMINO_TYPES;
}

int new_shape() {
  int v;
  current_shape.index = next_shape;
  draw_tetramino();
  current_shape.column_index = NUMBER_OF_COLUMNS / 2;
  current_shape.row_index = 0;
  current_shape.rotation_angle = 0;
  v = valid_position();
  if (v) {
    fill_current_shape(current_shape.index + 1);
    gtk_widget_queue_draw(window);
  }
  return v;
}

bool complete_row(unsigned int row_index) {
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



void detect_lines() {
  unsigned int row_index;
  unsigned int count = 0;
  for (row_index = 0; row_index < NUMBER_OF_ROWS; row_index++) {
    if (complete_row(row_index)) {
      remove_row(row_index);
      count++;
    }
  }
  if (count > 0) {
    score += 1 << (count - 1);
    update_score();
  }
}


void set_grid_to_zero() {
  unsigned int row_index, column_index;
  for (row_index = 0; row_index < NUMBER_OF_ROWS; row_index++) {
    for (column_index = 0; column_index < NUMBER_OF_COLUMNS; column_index++) {
      grid[row_index][column_index] = 0;
    }
  }
}


int main(void) {
  GtkWidget *button_newgame, *hbox, *vbox, *drawing_area, *next_piece;
  gtk_init(0, NULL);
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
  hbox = gtk_hbox_new(TRUE, 10);
  gtk_container_add(GTK_CONTAINER(window), hbox);
  drawing_area = gtk_drawing_area_new();
  gtk_container_add(GTK_CONTAINER(hbox), drawing_area);
  gtk_widget_set_size_request(drawing_area, NUMBER_OF_COLUMNS * SQUARE_SIDE_LENGTH, NUMBER_OF_ROWS * SQUARE_SIDE_LENGTH);
  g_signal_connect(G_OBJECT(drawing_area), "realize", G_CALLBACK(realize), NULL);
  g_signal_connect(G_OBJECT(drawing_area), "expose_event", G_CALLBACK(drawing_area_expose_event), NULL);
  vbox = gtk_vbox_new(TRUE, 10);
  gtk_container_add(GTK_CONTAINER(hbox), vbox);
  button_newgame = gtk_button_new_with_label ("New game");
  g_signal_connect(button_newgame, "clicked", G_CALLBACK(button_newgame_clicked), NULL);
  gtk_container_add(GTK_CONTAINER(vbox), button_newgame);
  score_label = gtk_label_new("");
  gtk_container_add(GTK_CONTAINER (vbox), score_label);
  next_piece = gtk_drawing_area_new();
  gtk_container_add(GTK_CONTAINER(vbox), next_piece);
  gtk_widget_set_size_request(next_piece, 4 * SQUARE_SIDE_LENGTH, 4 * SQUARE_SIDE_LENGTH);
  g_signal_connect(G_OBJECT(next_piece), "realize", G_CALLBACK(realize), NULL);
  g_signal_connect(G_OBJECT(next_piece), "expose_event", G_CALLBACK(next_piece_expose_event), NULL);
  g_signal_connect(G_OBJECT(window), "key_press_event", G_CALLBACK(key_press_event), NULL);
  new_game();
  gtk_widget_show(drawing_area);
  gtk_widget_show(next_piece);
  gtk_widget_show(button_newgame);
  gtk_widget_show(score_label);
  gtk_widget_show(hbox);
  gtk_widget_show(vbox);
  gtk_widget_show(window);
  g_timeout_add(500, timeout, NULL);
  gtk_main();
  return EXIT_SUCCESS;
}
