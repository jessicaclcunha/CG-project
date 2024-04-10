#include "matrix.hpp"
#include <stdlib.h>

typedef struct matrix {
    int rows;
    int cols;
    float **values;
} *MATRIX;

MATRIX create_matrix(int rows, int cols) {
    MATRIX m = (MATRIX)malloc(sizeof(MATRIX));
    m->rows = rows;
    m->cols = cols;
    m->values = (float **)malloc(rows * sizeof(float *));
    for (int i = 0; i < rows; i++) {
        m->values[i] = (float *)malloc(cols * sizeof(float));
    }
    return m;
}

void free_matrix(MATRIX m) {
    for (int i = 0; i < m->rows; i++) {
        free(m->values[i]);
    }
    free(m->values);
    free(m);
}

MATRIX matrix_multiplication(MATRIX a, MATRIX b) {
    if (a->cols != b->rows) {
        return NULL;
    }
    MATRIX result = create_matrix(a->rows, b->cols);
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            result->values[i][j] = 0;
            for (int k = 0; k < a->cols; k++) {
                result->values[i][j] += a->values[i][k] * b->values[k][j];
            }
        }
    }
    return result;
}