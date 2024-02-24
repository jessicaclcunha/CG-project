// ponto.hpp
#ifndef PONTO_H
#define PONTO_H

#include <stdlib.h>
#include <math.h>

typedef struct ponto PONTO;

PONTO* new_ponto (float x, float y, float z);

float get_X (PONTO *p);

float get_Y (PONTO *p);

float get_Z (PONTO *p);

float distance_2_origin(PONTO* p);
#endif // PONTO_H
