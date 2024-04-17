// matrix.hpp
#ifndef MATRIX_H
#define MATRIX_H


#include <stdlib.h>
#include <vector>

using namespace std;

typedef struct matrix  *MATRIX;

MATRIX create_matrix(int rows, int cols, float **values);

void free_matrix(MATRIX m);

MATRIX matrix_multiplication(MATRIX a, MATRIX b);

float dot_product(MATRIX a, MATRIX b);

#endif // MATRIX_H