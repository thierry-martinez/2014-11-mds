#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "tetrominos.h"
#include "grid.h"
#include "window.h"

int main(void) {
  
  GtkWidget *button_newgame, *hbox, *vbox, *drawing_area, *next_piece;

  gtk_init(0, NULL);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

  /* Horizontal box */
  hbox = gtk_hbox_new(TRUE, 10);
  gtk_container_add(GTK_CONTAINER(window), hbox);

  /* Drawing area */
  drawing_area = gtk_drawing_area_new();
  gtk_container_add(GTK_CONTAINER(hbox), drawing_area);
  gtk_widget_set_size_request(drawing_area, NUMBER_OF_COLUMNS * SQUARE_SIDE_LENGTH, NUMBER_OF_ROWS * SQUARE_SIDE_LENGTH);
  g_signal_connect(G_OBJECT(drawing_area), "realize", G_CALLBACK(realize), NULL);
  g_signal_connect(G_OBJECT(drawing_area), "expose_event", G_CALLBACK(drawing_area_expose_event), NULL);

  /* Vertical box */
  vbox = gtk_vbox_new(TRUE, 10);
  gtk_container_add(GTK_CONTAINER(hbox), vbox);

  /* New game button */
  button_newgame = gtk_button_new_with_label ("New game");
  g_signal_connect(button_newgame, "clicked", G_CALLBACK(button_newgame_clicked), NULL);
  gtk_container_add(GTK_CONTAINER(vbox), button_newgame);

  /* Score */
  score_label = gtk_label_new("");
  gtk_container_add(GTK_CONTAINER (vbox), score_label);

  /* Next piece */
  next_piece = gtk_drawing_area_new();
  gtk_container_add(GTK_CONTAINER(vbox), next_piece);
  gtk_widget_set_size_request(next_piece, 4 * SQUARE_SIDE_LENGTH, 4 * SQUARE_SIDE_LENGTH);
  g_signal_connect(G_OBJECT(next_piece), "realize", G_CALLBACK(realize), NULL);
  g_signal_connect(G_OBJECT(next_piece), "expose_event", G_CALLBACK(next_piece_expose_event), NULL);
  g_signal_connect(G_OBJECT(window), "key_press_event", G_CALLBACK(key_press_event), NULL);
  new_game();

  /* Show main window */
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
