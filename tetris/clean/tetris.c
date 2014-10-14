#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

#define number_of_columns 16
#define number_of_rows 16

const unsigned int block_width = 16;
const unsigned int block_height = 16;

unsigned int grid[number_of_rows][number_of_columns];

#define shape_count 5

unsigned int next_shape;

struct shape {
  unsigned int index;
  unsigned int x;
  unsigned int y;
  unsigned int o;
} current_shape;

struct shapes {
  int coords[4][2];
  int center[2];
  float colors[3];
} shapes[shape_count] = {
  { { { 0, 0 }, { 1, 0 }, { 2, 0 }, { 3, 0 } }, { 3, 0 }, { 1, 0, 0 } },
  { { { 0, 0 }, { 0, 1 }, { 1, 0 }, { 2, 0 } }, { 1, 1 }, { 0, 1, 0 } },
  { { { 0, 1 }, { 0, 0 }, { 1, 1 }, { 2, 1 } }, { 1, 1 }, { 0, 0, 1 } },
  { { { 0, 0 }, { 0, 1 }, { 1, 0 }, { 1, 1 } }, { 1, 1 }, { 1, 1, 0 } },
  { { { 0, 0 }, { 1, 0 }, { 1, 1 }, { 2, 0 } }, { 2, 1 }, { 1, 0, 1 } } };

GtkWidget *window;

gboolean realize(GtkWidget *widget, gpointer data) {
  gtk_widget_queue_draw(widget); 
  return TRUE;
}

void fill_rectangle(cairo_t *cr, int c, int i, int j) {
  const int p = 2;
  cairo_rectangle(cr, j * block_width + p, i * block_height + p, block_width - p, block_height - p);
  float red   = shapes[c].colors[0];
  float green = shapes[c].colors[1];
  float blue  = shapes[c].colors[2];
  cairo_set_source_rgb(cr, red, green, blue);
  cairo_fill_preserve(cr);
  cairo_set_line_width(cr, p);
  cairo_set_source_rgb(cr, red * 0.5, green * 0.5, blue * 0.5);
  cairo_stroke(cr);
}

gboolean drawing_area_expose_event(GtkWidget *widget, gpointer data) {
  int row_index, column_index;
  
  cairo_t* cr = gdk_cairo_create (widget->window);
  cairo_set_source_rgb(cr, 1, 1, 1);
  cairo_paint(cr);

  for (row_index = 0; row_index < number_of_rows; row_index++) {
    for (column_index = 0; column_index < number_of_columns; column_index++) {
      int c = grid[row_index][column_index];
      if (c != 0) {
        fill_rectangle(cr, c - 1, row_index, column_index);
      }
    }
  }
  cairo_destroy(cr);
  return TRUE;
}

gboolean next_piece_expose_event(GtkWidget *widget, gpointer data) {
  int k;
  
  cairo_t* cr = gdk_cairo_create (widget->window);
  cairo_set_source_rgb(cr, 1, 1, 1);
  cairo_paint(cr);

  for (k = 0; k < 4; k++) {
    int j = 2 + shapes[next_shape].coords[k][1];
    int i = shapes[next_shape].coords[k][0];
    fill_rectangle(cr, next_shape, i, j);
  }
  cairo_destroy(cr);
  return TRUE;
}

int get_shape_x(unsigned int i) {
  int oy = shapes[current_shape.index].coords[i][0];
  int ox = shapes[current_shape.index].coords[i][1];
  int cy = shapes[current_shape.index].center[0];
  int cx = shapes[current_shape.index].center[1];
  int x;
  if (current_shape.o == 0) {
    x = ox;
  }
  if (current_shape.o == 1) {
    x = oy;
  }
  if (current_shape.o == 2) {
    x = cx - ox;
  }
  if (current_shape.o == 3) {
    x = cy - oy;
  }
  x += current_shape.x;
}

int get_shape_y(unsigned int i) {
  int oy = shapes[current_shape.index].coords[i][0];
  int ox = shapes[current_shape.index].coords[i][1];
  int cy = shapes[current_shape.index].center[0];
  int cx = shapes[current_shape.index].center[1];
  int y;
  if (current_shape.o == 0) {
    y = oy;
  }
  if (current_shape.o == 1) {
    y = cx - ox;
  }
  if (current_shape.o == 2) {
    y = cy - oy;
  }
  if (current_shape.o == 3) {
    y = ox;
  }
  y += current_shape.y;
}

void fill_current_shape(unsigned int color) {
  unsigned int i;
  for (i = 0; i < 4; i++) {
    int y = get_shape_y(i);
    int x = get_shape_x(i);
    grid[y][x] = color;
  }
}

bool valid_position() {
  unsigned int i;
  for (i = 0; i < 4; i++) {
    int y = get_shape_y(i);
    int x = get_shape_x(i);
    if (!(x >= 0 && y >= 0 && x < number_of_columns && y < number_of_rows && grid[y][x] == 0)) {
      return false;
    }
  }
  return true;
}

int move_shape(int x, int y, int o) {
  struct shape old_shape = current_shape;
  fill_current_shape(0);
  current_shape.x += x;
  current_shape.y += y;
  current_shape.o += o;
  if (current_shape.o > 3) {
    current_shape.o -= 4;
  }
  if (current_shape.o < 0) {
    current_shape.o += 4;
  }
  int v = valid_position();
  if (!v) {
    current_shape = old_shape;
  }
  fill_current_shape(current_shape.index + 1);
  gtk_widget_queue_draw(window);
  return v;
}

void peek_next_shape() {
  next_shape = rand() % shape_count;
}

int new_shape() {
  int v;
  current_shape.index = next_shape;
  peek_next_shape();
  current_shape.x = number_of_columns / 2;
  current_shape.y = 0;
  current_shape.o = 0;
  v = valid_position();
  if (v) {
    fill_current_shape(current_shape.index + 1);
    gtk_widget_queue_draw(window);
  }
  return v;
}

bool complete_row(unsigned int row_index) {
  unsigned int column_index;
  for (column_index = 0; column_index < number_of_columns; column_index++) {
    if (grid[row_index][column_index] == 0) {
      return false;
    }
  }
  return true;
}

void remove_row(unsigned int removed_row_index) {
  unsigned int row_index, column_index;
  for (row_index = removed_row_index; row_index > 0; row_index--) {
    for (column_index = 0; column_index < number_of_columns; column_index++) {
      grid[row_index][column_index] = grid[row_index-1][column_index];
    }
  }
  for (column_index = 0; column_index < number_of_columns; column_index++) {
    grid[0][column_index] = 0;
  }
}

GtkWidget *score_label;

unsigned int score = 0;

void update_score() {
  char score_text[255];
  sprintf(score_text, "Score: %u", score);
  gtk_label_set_text(GTK_LABEL(score_label), score_text);
}

void detect_lines() {
  unsigned int row_index;
  unsigned int count = 0;
  for (row_index = 0; row_index < number_of_rows; row_index++) {
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

void set_grid_to_zero() {
  unsigned int row_index, column_index;
  for (row_index = 0; row_index < number_of_rows; row_index++) {
    for (column_index = 0; column_index < number_of_columns; column_index++) {
      grid[row_index][column_index] = 0;
    }
  }
}

void new_game() {
  set_grid_to_zero();
  srand(time(NULL));
  peek_next_shape();
  new_shape();
  fill_current_shape(current_shape.index + 1);
  update_score();
  gtk_widget_queue_draw(window);
}

gboolean button_newgame_clicked(GtkWidget *widget, gpointer data) {
  new_game();
  return TRUE;
}

int main(int argc, char *argv[]) {
  GtkWidget *button_newgame, *hbox, *vbox, *drawing_area, *next_piece;
  gtk_init(&argc, &argv);
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
  hbox = gtk_hbox_new(TRUE, 10);
  gtk_container_add(GTK_CONTAINER(window), hbox);
  drawing_area = gtk_drawing_area_new();
  gtk_container_add(GTK_CONTAINER(hbox), drawing_area);
  gtk_widget_set_size_request(drawing_area, number_of_columns * block_width, number_of_rows * block_height);
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
  gtk_widget_set_size_request(next_piece, 4 * block_width, 4 * block_height);
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
