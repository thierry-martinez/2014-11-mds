#include "window.h"

struct drawing_area_spec {
  GtkWidget *container;
  unsigned int width, height;
  GCallback expose_event;
};

GtkWidget *new_drawing_area(struct drawing_area_spec spec) {
  GtkWidget *result = gtk_drawing_area_new();
  gtk_container_add(GTK_CONTAINER(spec.container), result);
  gtk_widget_set_size_request(result, spec.width, spec.height);
  g_signal_connect(G_OBJECT(result), "realize", G_CALLBACK(realize), NULL);
  g_signal_connect(G_OBJECT(result), "expose_event", spec.expose_event, NULL);
  return result;
}

void initialize_horizontal_box() {
  application.hbox = gtk_hbox_new(TRUE, 10);
  gtk_container_add(GTK_CONTAINER(application.window), application.hbox);
  gtk_widget_show(application.hbox);
}

void initialize_grid() {
  struct drawing_area_spec grid_spec;
  grid_spec.container = application.hbox;
  grid_spec.width = NUMBER_OF_COLUMNS * SQUARE_SIDE_LENGTH;
  grid_spec.height = NUMBER_OF_ROWS * SQUARE_SIDE_LENGTH;
  grid_spec.expose_event = G_CALLBACK(grid_expose_event);
  application.grid = new_drawing_area(grid_spec);
  gtk_widget_show(application.grid);
}

void initialize_vertical_box() {
  application.vbox = gtk_vbox_new(TRUE, 10);
  gtk_container_add(GTK_CONTAINER(application.hbox), application.vbox);
  gtk_widget_show(application.vbox);
}

void initialize_button_newgame() {
  application.button_newgame = gtk_button_new_with_label("New game");
  g_signal_connect(application.button_newgame, "clicked",
                   G_CALLBACK(button_newgame_clicked), NULL);
  gtk_container_add(GTK_CONTAINER(application.vbox), application.button_newgame);
  gtk_widget_show(application.button_newgame);
}

void initialize_score_label() {
  application.score_label = gtk_label_new("");
  gtk_container_add(GTK_CONTAINER (application.vbox), application.score_label);
  gtk_widget_show(application.score_label);
}

void initialize_next_piece() {
  struct drawing_area_spec next_piece_spec;
  next_piece_spec.container = application.vbox;
  next_piece_spec.width = 4 * SQUARE_SIDE_LENGTH;
  next_piece_spec.height = 4 * SQUARE_SIDE_LENGTH;
  next_piece_spec.expose_event = G_CALLBACK(next_piece_expose_event);
  application.next_piece = new_drawing_area(next_piece_spec);
  gtk_widget_show(application.next_piece);  
}

void initialize_window() {
  application.window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  g_signal_connect(application.window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
  g_signal_connect(G_OBJECT(application.window), "key_press_event", G_CALLBACK(key_press_event), NULL);
  gtk_widget_show(application.window);
}

void initialize_application() {
  gtk_init(0, NULL);

  initialize_window();
  initialize_horizontal_box();
  initialize_grid();
  initialize_vertical_box();
  initialize_button_newgame();
  initialize_score_label();
  initialize_next_piece();

  new_game();
}

void fill_cell(cairo_t *cr, int tetromino_type, int i, int j) {
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

void draw_grid() {
  int row_index, column_index;
  
  cairo_t* cr = gdk_cairo_create (application.grid->window);
  cairo_set_source_rgb(cr, WHITE.red, WHITE.green, WHITE.blue);
  cairo_paint(cr);

  for (row_index = 0; row_index < NUMBER_OF_ROWS; row_index++) {
    for (column_index = 0; column_index < NUMBER_OF_COLUMNS; column_index++) {
      int tetromino_type = grid[row_index][column_index];
      if (tetromino_type != 0) {
        fill_cell(cr, tetromino_type - 1, row_index, column_index);
      }
    }
  }
  cairo_destroy(cr);
}

gboolean realize(GtkWidget *widget, gpointer data) {
  gtk_widget_queue_draw(widget);
  return TRUE;
}

gboolean grid_expose_event(GtkWidget *widget, gpointer data) {
  draw_grid();
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
    fill_cell(cr, next_shape, i, j);
  }
  cairo_destroy(cr);
  return TRUE;
}

void update_score() {
  char score_text[255];
  sprintf(score_text, "Score: %u", score);
  gtk_label_set_text(GTK_LABEL(application.score_label), score_text);
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
    move_shape_to_bottom();
    break;
  }
  return TRUE;
}

void redraw() {
  gtk_widget_queue_draw(application.window);
}

void new_game() {
  set_grid_to_zero();
  srand(time(NULL));
  draw_tetramino();
  new_shape();
  fill_current_shape(current_shape.index + 1);
  update_score();
  gtk_widget_queue_draw(application.window);
  g_timeout_add(500, timeout, NULL);
}

gboolean button_newgame_clicked(GtkWidget *widget, gpointer data) {
  new_game();
  return TRUE;
}

unsigned int score = 0;

