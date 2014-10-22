#ifndef TETRIS_RGB_COLOR_H
#define TETRIS_RGB_COLOR_H

#define RED    {1,0,0}
#define GREEN  {0,1,0}
#define BLUE   {0,0,1}
#define YELLOW {1,1,0}
#define PURPLE {1,0,1}

typedef struct rgb_color {
  float red;
  float green;
  float blue;
} rgb_color;

rgb_color const WHITE = {1,1,1};

#endif
