#include <stdlib.h>
#include "point.hpp"
#include <math.h>

typedef struct point {
    float x;
    float y;
    float z;
} *POINT;

POINT new_point (float x, float y, float z) {
    POINT p = (POINT )malloc (sizeof(struct point));
    if (p != NULL)
    {
        p->x = x;
        p->y = y;
        p->z = z;
    } 
    return p;
}

float get_X (POINT p) {
    if (p != NULL)
        return p->x;
}

float get_Y (POINT p) {
    if (p != NULL)
        return p->y;
}

float get_Z (POINT p) {
    if (p != NULL)
        return p->z;
}

float distance_2_origin(POINT p) {
    return sqrt(p->x * p->x + p->y * p->y + p->z * p->z);
}