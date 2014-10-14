gboolean realize(GtkWidget *widget, gpointer data)
void fill_rectangle(cairo_t *cr, int c, int i, int j)
gboolean drawing_area_expose_event(GtkWidget *widget, gpointer data)
gboolean next_piece_expose_event(GtkWidget *widget, gpointer data)

int get_shape_x(unsigned int i)
int get_shape_y(unsigned int i)
void fill_current_shape(unsigned int color)
int valid_position()
int move_shape(int x, int y, int o)
void peek_next_shape()
int new_shape()

int complete_line(i)
void remove_line(i)
void detect_lines()

void update_score()
gint timeout(gpointer data)
gboolean key_press_event(GtkWidget *widget, GdkEventKey *event, gpointer data)
void new_game()
gboolean button_newgame_clicked(GtkWidget *widget, gpointer data)
int main(int argc, char *argv[])

