#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "tetrominos.h"
#include "grid.h"
#include "window.h"


int main(void) {
  srand(time(NULL));
  initialize_application();
  gtk_main();
  return EXIT_SUCCESS;
}
