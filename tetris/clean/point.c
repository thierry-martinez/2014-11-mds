
#include "point.h"

#include <math.h>

int getAbscissa(Point const point)
{ return point.coordinates[0]; }

int getOrdinate(Point const point)
{ return point.coordinates[1]; }

void setAbscissa(Point * const point, int const abscissa)
{ point->coordinates[0] = abscissa; }

void setOrdinate(Point * const point, int const ordinate)
{ point->coordinates[1] = ordinate; }

void rotate90(Point const center, Point * const point)
{
  /* plane rotation formulas are simplified when angle is 90
   * Reminder : plane rotation formulas are 
   *            new_x = cx + cos(t)*(x-cx) - sin(t)*y-cy)
   *            new_y = cy + cos(t)*(y-cy) + sin(t)*(x-cx)
   *
   * with cos(t)=0 and sin(t)=1 if t=90
   */
  int new_abscissa = getAbscissa(center) + getOrdinate(center) - getOrdinate(*point);
  int new_ordinate = getOrdinate(center) + getAbscissa(*point) - getAbscissa(center);

  setAbscissa(point, new_abscissa);
  setOrdinate(point, new_ordinate);
}

bool samePoints(Point const left, Point const right)
{
  if ( getAbscissa(left) != getAbscissa(right) )
    return false;
  return getOrdinate(left) == getOrdinate(right);
}

