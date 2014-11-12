#ifndef TETRIS_RGB_COLOR_H
#define TETRIS_RGB_COLOR_H

#define RGB(R,G,B) {R / 255.0, G / 255.0, B / 255.0}
	
#define RED    RGB(0xc0, 0x39, 0x2b)
#define GREEN  RGB(0x27, 0xae, 0x60)
#define BLUE   RGB(0x29, 0x80, 0xb9)
#define YELLOW RGB(0xf3, 0x9c, 0x12)
#define PURPLE RGB(0x8e, 0x44, 0xad)
#define ORANGE RGB(0xd3, 0x54, 0x00)
#define TURQUOISE	RGB(0x1a, 0xbc, 0x9c)

typedef struct rgb_color {
  float red;
  float green;
  float blue;
} rgb_color;

static rgb_color const WHITE = {1,1,1};

#endif
