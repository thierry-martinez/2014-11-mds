
#ifndef TETRIS_POINT_H
#define TETRIS_POINT_H

#include <stdbool.h>

struct point
{
  int coordinates[2];
};

typedef struct point Point;

int getAbscissa(Point const point);
int getOrdinate(Point const point);

void setAbscissa(Point * const point, int const abscissa);
void setOrdinate(Point * const point, int const ordinate);

void rotate90(Point const center, Point * const point);

bool samePoints(Point const left, Point const right);

#endif
