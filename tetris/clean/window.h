#ifndef TETRIS_WINDOW_H
#define TETRIS_WINDOW_H

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

#include "grid.h"
#include "tetrominos.h"



GtkWidget *window;
GtkWidget *score_label;

static unsigned int score = 0;

void fill_rectangle(cairo_t *cr, int tetromino_type, int i, int j);
gboolean realize(GtkWidget *widget, gpointer data);
gboolean drawing_area_expose_event(GtkWidget *widget, gpointer data);
gboolean next_piece_expose_event(GtkWidget *widget, gpointer data);
void update_score();
gint timeout(gpointer data);
gboolean key_press_event(GtkWidget *widget, GdkEventKey *event, gpointer data);
void new_game();
gboolean button_newgame_clicked(GtkWidget *widget, gpointer data);

#endif
