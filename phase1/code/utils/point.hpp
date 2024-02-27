// point.hpp
#ifndef POINT_H
#define POINT_H

#include <stdlib.h>
#include <math.h>

typedef struct point *POINT;

POINT new_point (float x, float y, float z);

float get_X (POINT p);

float get_Y (POINT p);

float get_Z (POINT p);

float distance_2_origin(POINT p);
#endif // POINT_H
