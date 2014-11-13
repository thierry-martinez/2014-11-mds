#include "window.h"

/* Score */

unsigned int get_score(void) {
  return game.score;
}

void set_score(unsigned int new_score) {
  char score_text[255];
  game.score = new_score;
  sprintf(score_text, "Score: %u",  game.score);
  gtk_label_set_text(GTK_LABEL(application.score_label), score_text);
}

/* Initialization */

struct drawing_area_spec {
  GtkWidget *container;
  unsigned int width, height;
  GCallback expose_event;
};

void signal_connect(GtkWidget *widget, char *event_name, GCallback callback) {
  g_signal_connect(G_OBJECT(widget), event_name, callback, NULL);
}

GtkWidget *new_drawing_area(struct drawing_area_spec spec) {
  GtkWidget *result = gtk_drawing_area_new();
  gtk_container_add(GTK_CONTAINER(spec.container), result);
  gtk_widget_set_size_request(result, spec.width, spec.height);
  signal_connect(result, "realize", G_CALLBACK(on_realize_event));
  signal_connect(result, "expose_event", spec.expose_event);
  return result;
}

void new_game() {
  set_grid_to_zero();
  next_tetromino_type = random_tetromino_type();
  new_shape();
  fill_current_shape(current_shape.index + 1);
  set_score(0);

  set_state(Normal);
  update_pause_button();

  gtk_widget_queue_draw(application.window);
}

void initialize_horizontal_box() {
  application.horizontalLayout = gtk_hbox_new(TRUE, 10);
  gtk_container_add(GTK_CONTAINER(application.window), application.horizontalLayout);
  gtk_widget_show(application.horizontalLayout);
}

void initialize_vertical_box() {
  application.verticalLayout = gtk_vbox_new(TRUE, 10);
  gtk_container_add(GTK_CONTAINER(application.horizontalLayout), application.verticalLayout);
  gtk_widget_show(application.verticalLayout);
}

void initialize_buttons() {
  GtkWidget *bbox = gtk_hbox_new(TRUE, 10);
  gtk_container_add(GTK_CONTAINER(application.verticalLayout), bbox);

  application.button_newgame = gtk_button_new_with_label("New game");
  application.button_pause = gtk_button_new_with_label("Pause");

  g_signal_connect
    (application.button_newgame, "clicked",
     G_CALLBACK(on_button_newgame_click_event), NULL);
  g_signal_connect
    (application.button_pause, "clicked",
     G_CALLBACK(on_button_pause_click_event), NULL);

  gtk_container_add
    (GTK_CONTAINER(bbox), application.button_newgame);
  gtk_container_add
    (GTK_CONTAINER(bbox), application.button_pause);

  gtk_widget_show(bbox);
  gtk_widget_show(application.button_newgame);
  gtk_widget_show(application.button_pause);
}

void initialize_score_label() {
  application.score_label = gtk_label_new("");
  gtk_container_add(GTK_CONTAINER(application.verticalLayout), application.score_label);
  gtk_widget_show(application.score_label);
}

void initialize_next_piece() {
  struct drawing_area_spec next_piece_spec;
  next_piece_spec.container = application.verticalLayout;
  next_piece_spec.width = 4 * SQUARE_SIDE_LENGTH;
  next_piece_spec.height = 4 * SQUARE_SIDE_LENGTH;
  next_piece_spec.expose_event = G_CALLBACK(on_next_piece_expose_event);
  application.next_piece = new_drawing_area(next_piece_spec);
  gtk_widget_show(application.next_piece);
}

void initialize_window() {
  application.window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  g_signal_connect
    (application.window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
  g_signal_connect
    (G_OBJECT(application.window), "key_press_event",
     G_CALLBACK(on_key_press_event), NULL);
  gtk_widget_show(application.window);

  gtk_window_set_position(GTK_WINDOW(application.window), GTK_WIN_POS_CENTER_ALWAYS);
  gtk_window_set_resizable(GTK_WINDOW(application.window), FALSE);
}

void initialize_layout() {
  initialize_horizontal_box();
  initialize_vertical_box();
}

void initialize_application() {
  gtk_init(0, NULL);

  initialize_window();
  initialize_layout();
  initialize_grid();
  initialize_buttons();
  initialize_score_label();
  initialize_next_piece();

  g_timeout_add(500, on_timeout_event, NULL);
  new_game();
}

void initialize_grid() {
  struct drawing_area_spec grid_spec;
  grid_spec.container = application.horizontalLayout;
  grid_spec.width = NUMBER_OF_COLUMNS * SQUARE_SIDE_LENGTH;
  grid_spec.height = NUMBER_OF_ROWS * SQUARE_SIDE_LENGTH;
  grid_spec.expose_event = G_CALLBACK(on_grid_expose_event);
  application.grid = new_drawing_area(grid_spec);
  gtk_widget_show(application.grid);
}

void update_pause_button() {
  if (game.state == Normal) {
    gtk_button_set_label(GTK_BUTTON(application.button_pause), "Pause");
  } else if (game.state == Paused) {
    gtk_button_set_label(GTK_BUTTON(application.button_pause), "Resume");
  }
}

/* Drawing */

void fill_cell(cairo_t *cr, int tetromino_type, int i, int j) {
  const int line_width = 2;
  double top_left_abscissa = j * SQUARE_SIDE_LENGTH + line_width;
  double top_left_ordinate = i * SQUARE_SIDE_LENGTH + line_width;
  double width = SQUARE_SIDE_LENGTH - line_width;
  double height = SQUARE_SIDE_LENGTH - line_width;
  cairo_rectangle(cr, top_left_abscissa, top_left_ordinate, width, height);
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
      int tetromino_type = game.grid[row_index][column_index];
      if (tetromino_type != 0) {
        fill_cell(cr, tetromino_type - 1, row_index, column_index);
      }
    }
  }
  cairo_destroy(cr);
}

void draw_next_piece(void) {
  int square_index;
  
  cairo_t* cr = gdk_cairo_create (application.next_piece->window);
  cairo_set_source_rgb(cr, 1, 1, 1);
  cairo_paint(cr);

  for (square_index = 0; square_index < NUMBER_OF_SQUARES; square_index++) {
    int i = getAbscissa(tetrominos[next_tetromino_type].coords[square_index]);
    int j = 2 + getOrdinate(tetrominos[next_tetromino_type].coords[square_index]);
    fill_cell(cr, next_tetromino_type, i, j);
  }
  cairo_destroy(cr);
}

void redraw() {
  gtk_widget_queue_draw(application.window);
}

/* Events */

gint on_timeout_event(gpointer data) {
  if (get_state() == Normal) {
    if (!(move_shape(0, 1, 0))) {
      detect_lines();
      if (!new_shape()) {
	  set_state(Finished);
      }
    }
  }
  g_timeout_add(500, on_timeout_event, NULL);
  return 0;
}

gboolean on_realize_event(GtkWidget *widget, gpointer data) {
  gtk_widget_queue_draw(widget);
  return TRUE;
}

gboolean on_grid_expose_event(GtkWidget *widget, gpointer data) {
  draw_grid();
  return TRUE;
}

gboolean on_next_piece_expose_event(GtkWidget *widget, gpointer data) {
  draw_next_piece();
  return TRUE;
}

gboolean on_key_press_event(GtkWidget *widget,
                            GdkEventKey *event,
                            gpointer data) {
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

gboolean on_button_newgame_click_event(GtkWidget *widget, gpointer data) {
  new_game();
  return TRUE;
}

gboolean on_button_pause_click_event(GtkWidget *widget, gpointer data) {
  if (game.state == Normal) {
    game.state = Paused;
  } else if (game.state == Paused) {
    game.state = Normal;
  }
  update_pause_button();
  return TRUE;
}
