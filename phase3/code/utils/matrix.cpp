#include "matrix.hpp"
#include <stdlib.h>

using namespace std;

typedef struct matrix {
    int rows;
    int cols;
    float **values;
} *MATRIX;

MATRIX create_matrix(int rows, int cols, float **values) {
    MATRIX m = (MATRIX)malloc(sizeof(MATRIX));
    m->rows = rows;
    m->cols = cols;
    m->values = (float**)malloc(rows * sizeof(float*));
    for (int i = 0; i < rows; i++) {
        m->values[i] = (float*)malloc(cols * sizeof(float));
        if (values != NULL) {
            for (int j = 0; j < cols; j++) {
                m->values[i][j] = values[i][j];
            }
        }
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
    MATRIX result = create_matrix(a->rows, b->cols, NULL);
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

float dot_product(MATRIX a, MATRIX b) {
    float result = 0.0f;
    for (int i = 0; i < a->rows; ++i) {
        for (int j = 0; j < a->cols; ++j) {
            result += a->values[i][j] * b->values[i][j];
        }
    }
    return result;
}

float** copy_values(float **matrix, int rows, int cols) {
    float **copy = (float**)malloc(rows * sizeof(float*));
    for (int i = 0; i < rows; i++) {
        copy[i] = (float*)malloc(cols * sizeof(float));
        for (int j = 0; j < cols; j++) {
            copy[i][j] = matrix[i][j];
        }
    }
    return copy;
}

POINT catmullRom_curve(std::vector<POINT> points, float t) {
    int numPoints = points.size();

    printf("%d", numPoints);
    // Check if there are enough points to calculate the curve
    if (numPoints <= 3) {
        std::cerr << "Error: Insufficient points to calculate the Catmull-Rom curve." << std::endl;
        return POINT();
    }

    printf("Aqui1");
    // Determine the segment of the curve
    int segment = std::min(std::max(0, static_cast<int>(t * (numPoints - 3))), numPoints - 4);
    printf("Aqui2" );
    // Calculate the parameter t for the segment
    float segmentT = t * (numPoints - 3) - segment;
    printf("Aqui3");
    // Calculate the basis matrix
    float basisValues[4][4] = {
        {-0.5f,  1.5f, -1.5f,  0.5f},
        { 1.0f, -2.5f,  2.0f, -0.5f},
        {-0.5f,  0.0f,  0.5f,  0.0f},
        { 0.0f,  1.0f,  0.0f,  0.0f}
    };
    printf("Aqui4");
    // Create and initialize the basis matrix
    float **basisValuesPtr = new float*[4];
    for (int i = 0; i < 4; ++i) {
        basisValuesPtr[i] = new float[4];
        for (int j = 0; j < 4; ++j) {
            basisValuesPtr[i][j] = basisValues[i][j];
        }
    }
    printf("Aqui5");
    MATRIX basisMatrix = create_matrix(4, 4, basisValuesPtr);
    printf("Aqui6");
    // Calculate the parameter vector
    float parameterValues[1][4] = {{1, segmentT, segmentT * segmentT, segmentT * segmentT * segmentT}};
    printf("Aqui7");
    // Create and initialize the parameter vector
    float **parameterValuesPtr = new float*[1];
    parameterValuesPtr[0] = new float[4];
    for (int j = 0; j < 4; ++j) {
        parameterValuesPtr[0][j] = parameterValues[0][j];
    }
    printf("Aqui8");
    MATRIX parameterVector = create_matrix(1, 4, parameterValuesPtr);
    printf("Aqui9");

    // Calculate the control points matrix
    float** controlPointsValues = (float**)malloc(3 * sizeof(float*));
    for (int i = 0; i < 3; ++i) {
        controlPointsValues[i] = (float*)malloc(4 * sizeof(float));
    }
    for (int i = 0; i < 4; ++i) {
        controlPointsValues[0][i] = get_X(points[segment + i]);
        controlPointsValues[1][i] = get_Y(points[segment + i]);
        controlPointsValues[2][i] = get_Z(points[segment + i]);
    }
    printf("Aqui10");
    // Create and initialize the control points matrix
    MATRIX controlPointsMatrix = create_matrix(3, 4, controlPointsValues);
    printf("Aqui11");
    // Perform the matrix multiplication
    MATRIX tempResult = matrix_multiplication(controlPointsMatrix, basisMatrix);
    MATRIX resultMatrix = matrix_multiplication(tempResult, parameterVector);

    // Extract the interpolated point from the result matrix
    POINT interpolatedPoint = new_point(resultMatrix->values[0][0], resultMatrix->values[1][0],  resultMatrix->values[2][0]);

    // Free allocated memory
    free_matrix(basisMatrix);
    free_matrix(parameterVector);
    free_matrix(controlPointsMatrix);
    free_matrix(tempResult);
    free_matrix(resultMatrix);

    return interpolatedPoint;
}


void buildRotMatrix(POINT x, POINT y, POINT z, float* result) {
    result[0] = get_X(x);  result[4] = get_Y(x);  result[8]  = get_Z(x);  result[12] = 0.0f;
    result[1] = get_X(y);  result[5] = get_Y(y);  result[9]  = get_Z(y);  result[13] = 0.0f;
    result[2] = get_X(z);  result[6] = get_Y(z);  result[10] = get_Z(z);  result[14] = 0.0f;
    result[3] = 0.0f ;     result[7] = 0.0f;      result[11] = 0.0f;      result[15] = 1.0f;
}