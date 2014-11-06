#ifndef TETRIS_WINDOW_H
#define TETRIS_WINDOW_H

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

#include "grid.h"
#include "tetrominos.h"

struct application {
  GtkWidget *button_newgame, *hbox, *vbox, *grid, *next_piece;
  GtkWidget *window;
  GtkWidget *score_label;
} application;

/* Score */

unsigned int get_score(void);
void set_score(unsigned int new_score);

/* Initialization */

void new_game(void);
void initialize_application(void);
void initialize_grid(void);

/* Drawing */

void fill_cell(cairo_t *cr, int tetromino_type, int i, int j);
void redraw(void);

/* Events */

gint on_timeout_event(gpointer data);
gboolean on_realize_event(GtkWidget *widget, gpointer data);
gboolean on_grid_expose_event(GtkWidget *widget, gpointer data);
gboolean on_next_piece_expose_event(GtkWidget *widget, gpointer data);

gboolean on_key_press_event(GtkWidget *widget,
                            GdkEventKey *event,
                            gpointer data);
gboolean on_button_newgame_click_event(GtkWidget *widget, gpointer data);

#endif
