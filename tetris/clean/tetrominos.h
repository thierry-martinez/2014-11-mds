#ifndef TETRIS_TETROMINOS_H
#define TETRIS_TETROMINOS_H

#include "rgb_color.h"
#include "point.h"

#define SPACE_DIMENSION 2
#define NUMBER_OF_SQUARES 4
#define NUMBER_OF_TETROMINO_TYPES 7

/* Official names of tetrominos
 * Do NOT modify these names */

/* (0,0) (1,0) (2,0) (3,0) */
#define SHAPE_I { {0,0}, {1,0}, {2,0}, {3,0} }
#define POLYOMINO_I {SHAPE_I, {3,0}, RED }

/* (0,0) (1,0) (2,0)
 * (0,1) */
#define SHAPE_L { {0,0}, {0,1}, {1,0}, {2,0} }
#define POLYOMINO_L {SHAPE_L, {1,1}, BLUE}

/* (0,0)
 * (0,1) (1,1) (2,1) */
#define SHAPE_J { {0,1}, {0,0}, {1,1}, {2,1} }
#define POLYOMINO_J {SHAPE_J, {1,1}, GREEN}

/* (0,0) (1,0)
 * (0,1) (1,1) */
#define SHAPE_O { {0,0}, {0,1}, {1,0}, {1,1} }
#define POLYOMINO_O {SHAPE_O, {1,1}, YELLOW}

/*   (0,1) (1,0) (2,0)
 *         (1,1) */
#define SHAPE_T { {0,0}, {1,0}, {1,1}, {2,0} }
#define POLYOMINO_T {SHAPE_T, {2,1}, PURPLE}

/*       (1,0) (2,0)
 * (0,1) (1,1) */
#define SHAPE_N { {0,1}, {1,1}, {1,0}, {2,0} }
#define POLYOMINO_N {SHAPE_N, {2,1}, TURQUOISE}

/* (0,0) (1,0)
 *       (1,1) (2,1) */
#define SHAPE_Z { {0,0}, {1,0}, {1,1}, {2,1} }
#define POLYOMINO_Z {SHAPE_Z, {2,1}, ORANGE}

enum RotationAngle { ANGLE_0=0, ANGLE_90=1, ANGLE_180=2, ANGLE_270=3 };

struct shape {
  unsigned int index;
  unsigned int column_index;
  unsigned int row_index;
  unsigned int rotation_angle;
} current_shape;

unsigned int next_shape;

static struct tetromino {
  Point coords[NUMBER_OF_SQUARES];
  Point rotation_center;
  rgb_color color;
} tetrominos[NUMBER_OF_TETROMINO_TYPES] = {
      POLYOMINO_I, POLYOMINO_J, POLYOMINO_L, POLYOMINO_O,
      POLYOMINO_T, POLYOMINO_Z, POLYOMINO_N };

#endif
