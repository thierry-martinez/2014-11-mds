

#include "window.h"

void fill_rectangle(cairo_t *cr, int tetromino_type, int i, int j) {
  const int line_width = 2;
  cairo_rectangle(cr, j * SQUARE_SIDE_LENGTH + line_width, i * SQUARE_SIDE_LENGTH + line_width, SQUARE_SIDE_LENGTH - line_width, SQUARE_SIDE_LENGTH - line_width);
  float red   = tetrominos[tetromino_type].color.red;
  float green = tetrominos[tetromino_type].color.green;
  float blue  = tetrominos[tetromino_type].color.blue;
  cairo_set_source_rgb(cr, red, green, blue);
  cairo_fill_preserve(cr);
  cairo_set_line_width(cr, line_width);
  cairo_set_source_rgb(cr, red * 0.5, green * 0.5, blue * 0.5);
  cairo_stroke(cr);
}

gboolean realize(GtkWidget *widget, gpointer data) {
  gtk_widget_queue_draw(widget);
  return TRUE;
}

gboolean drawing_area_expose_event(GtkWidget *widget, gpointer data) {
  int row_index, column_index;
  
  cairo_t* cr = gdk_cairo_create (widget->window);
  cairo_set_source_rgb(cr, WHITE.red, WHITE.green, WHITE.blue);
  cairo_paint(cr);

  for (row_index = 0; row_index < NUMBER_OF_ROWS; row_index++) {
    for (column_index = 0; column_index < NUMBER_OF_COLUMNS; column_index++) {
      int tetromino_type = grid[row_index][column_index];
      if (tetromino_type != 0) {
        fill_rectangle(cr, tetromino_type - 1, row_index, column_index);
      }
    }
  }
  cairo_destroy(cr);
  return TRUE;
}

gboolean next_piece_expose_event(GtkWidget *widget, gpointer data) {
  int square_index;
  
  cairo_t* cr = gdk_cairo_create (widget->window);
  cairo_set_source_rgb(cr, 1, 1, 1);
  cairo_paint(cr);

  for (square_index = 0; square_index < NUMBER_OF_SQUARES; square_index++) {
    int i = tetrominos[next_shape].coords[square_index][0];
    int j = 2 + tetrominos[next_shape].coords[square_index][1];
    fill_rectangle(cr, next_shape, i, j);
  }
  cairo_destroy(cr);
  return TRUE;
}

void update_score() {
  char score_text[255];
  sprintf(score_text, "Score: %u", score);
  gtk_label_set_text(GTK_LABEL(score_label), score_text);
}

gint timeout(gpointer data) {
  if (!(move_shape(0, 1, 0))) {
    detect_lines();
    if (!new_shape()) {
      return 0;
    }
  }
  g_timeout_add(500, timeout, NULL);
  return 0;
}

gboolean key_press_event(GtkWidget *widget, GdkEventKey *event, gpointer data) {
  switch (event->keyval) {
  case GDK_Left:
    move_shape(-1, 0, 0);
    break;
  case GDK_Right:
    move_shape(1, 0, 0);
    break;
  case GDK_Up:
    move_shape(0, 0, 1);
    break;
  case GDK_Down:
    move_shape(0, 1, 0);
    break;
  case GDK_KEY_space:
    while (move_shape(0, 1, 0));
    break;
  }
  return TRUE;
}

void new_game() {
  set_grid_to_zero();
  srand(time(NULL));
  draw_tetramino();
  new_shape();
  fill_current_shape(current_shape.index + 1);
  update_score();
  gtk_widget_queue_draw(window);
}

gboolean button_newgame_clicked(GtkWidget *widget, gpointer data) {
  new_game();
  return TRUE;
}

