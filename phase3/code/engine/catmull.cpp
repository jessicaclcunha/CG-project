#include "catmull.hpp"

void getCatmullRomPoint(float t, POINT p0, POINT p1, POINT p2, POINT p3, POINT pos, POINT deriv) {
    // Catmull-Rom matrix
    float m[4][4] = {
        {-0.5f,  1.5f, -1.5f,  0.5f},
        { 1.0f, -2.5f,  2.0f, -0.5f},
        {-0.5f,  0.0f,  0.5f,  0.0f},
        { 0.0f,  1.0f,  0.0f,  0.0f}
    };


    float p[4][3] = {
        {get_X(p0), get_Y(p0), get_Z(p0)},
        {get_X(p1), get_Y(p1), get_Z(p1)}, 
        {get_X(p2), get_Y(p2), get_Z(p2)},
        {get_X(p3), get_Y(p3), get_Z(p3)}
    };
                
    // Compute A = M * P
    float a[4][3];

    // Compute A = M * P
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 4; j++) {
            a[i][j] = 0;
            for (int k = 0; k < 4; k++) {
                a[i][j] += m[i][k] * p[k][j];
            }
        }
    }

    // Compute pos = T * A
    float pos_ptr[3] = {0.0f, 0.0f, 0.0f};
    float T[4] = {t * t * t, t * t, t, 1};
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 4; j++) {
            pos_ptr[i] += T[j] * a[i][j];
        }
    }

    if (pos != NULL)
    {
        set_X(pos, pos_ptr[0]);
        set_Y(pos, pos_ptr[1]);
        set_Z(pos, pos_ptr[2]);
    }
    
    // Compute deriv = T' * A
    float deriv_ptr[3] = {0.0f, 0.0f, 0.0f};
    float T_[4] = {3 * t * t, 2 * t, 1, 0};
    for (int i = 0; i < 3; i++) {
        deriv_ptr[i] = 0;
        for (int j = 0; j < 4; j++) {
            deriv_ptr[i] += T_[j] * a[i][j];
        }
    }

    if (deriv != NULL)
    {    set_X(deriv, deriv_ptr[0]);
        set_Y(deriv, deriv_ptr[1]);
        set_Z(deriv, deriv_ptr[2]);
    }
}

void getGlobalCatmullRomPoint(float gt, std::vector<POINT> control_points, POINT pos, POINT deriv) {
    size_t POINT_COUNT = control_points.size();
    float t = gt * POINT_COUNT; // This is the real global t
    int index = floor(t);       // Which segment
    t = t - index;              // Where within the segment

    // Indices store the points
    int indices[4];
    indices[0] = (index + POINT_COUNT - 1) % POINT_COUNT;
    indices[1] = (indices[0] + 1) % POINT_COUNT;
    indices[2] = (indices[1] + 1) % POINT_COUNT;
    indices[3] = (indices[2] + 1) % POINT_COUNT;

    // Compute Catmull-Rom point and derivative
    getCatmullRomPoint(t, control_points[indices[0]], control_points[indices[1]], control_points[indices[2]], control_points[indices[3]], pos, deriv);
}