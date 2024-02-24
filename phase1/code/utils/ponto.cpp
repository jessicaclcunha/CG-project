#include <stdlib.h>
#include "ponto.hpp"
#include <math.h>

typedef struct ponto {
    float x;
    float y;
    float z;
} PONTO;

PONTO* new_ponto (float x, float y, float z) {
    PONTO* p = (PONTO *)malloc (sizeof(struct ponto));
    if (p != NULL)
    {
        p->x = x;
        p->y = y;
        p->z = z;
    } 
    return p;
}

float get_X (PONTO *p) {
    if (p != NULL)
        return p->x;
}

float get_Y (PONTO *p) {
    if (p != NULL)
        return p->y;
}

float get_Z (PONTO *p) {
    if (p != NULL)
        return p->z;
}

float distance_2_origin(PONTO* p) {
    return sqrt(p->x * p->x + p->y * p->y + p->z * p->z);
}