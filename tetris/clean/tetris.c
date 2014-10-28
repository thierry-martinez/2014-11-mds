#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "tetrominos.h"
#include "grid.h"
#include "window.h"

int main(void) {
  

  gtk_init(0, NULL);

  application.window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  g_signal_connect(application.window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

  /* Horizontal box */
  application.hbox = gtk_hbox_new(TRUE, 10);
  gtk_container_add(GTK_CONTAINER(application.window), application.hbox);

  /* Drawing area */
  application.drawing_area = gtk_drawing_area_new();
  gtk_container_add(GTK_CONTAINER(application.hbox), application.drawing_area);
  gtk_widget_set_size_request(application.drawing_area, NUMBER_OF_COLUMNS * SQUARE_SIDE_LENGTH, NUMBER_OF_ROWS * SQUARE_SIDE_LENGTH);
  g_signal_connect(G_OBJECT(application.drawing_area), "realize", G_CALLBACK(realize), NULL);
  g_signal_connect(G_OBJECT(application.drawing_area), "expose_event", G_CALLBACK(drawing_area_expose_event), NULL);

  /* Vertical box */
  application.vbox = gtk_vbox_new(TRUE, 10);
  gtk_container_add(GTK_CONTAINER(application.hbox), application.vbox);

  /* New game button */
  application.button_newgame = gtk_button_new_with_label("New game");
  g_signal_connect(application.button_newgame, "clicked",
                   G_CALLBACK(button_newgame_clicked), NULL);
  gtk_container_add(GTK_CONTAINER(application.vbox), application.button_newgame);

  /* Score */
  application.score_label = gtk_label_new("");
  gtk_container_add(GTK_CONTAINER (application.vbox), application.score_label);

  /* Next piece */
  application.next_piece = gtk_drawing_area_new();
  gtk_container_add(GTK_CONTAINER(application.vbox), application.next_piece);
  gtk_widget_set_size_request(application.next_piece, 4 * SQUARE_SIDE_LENGTH, 4 * SQUARE_SIDE_LENGTH);
  g_signal_connect(G_OBJECT(application.next_piece), "realize", G_CALLBACK(realize), NULL);
  g_signal_connect(G_OBJECT(application.next_piece), "expose_event", G_CALLBACK(next_piece_expose_event), NULL);
  g_signal_connect(G_OBJECT(application.window), "key_press_event", G_CALLBACK(key_press_event), NULL);
  new_game();

  /* Show main window */
  gtk_widget_show(application.drawing_area);
  gtk_widget_show(application.next_piece);
  gtk_widget_show(application.button_newgame);
  gtk_widget_show(application.score_label);
  gtk_widget_show(application.hbox);
  gtk_widget_show(application.vbox);
  gtk_widget_show(application.window);
  
  gtk_main();
  
  return EXIT_SUCCESS;
}
