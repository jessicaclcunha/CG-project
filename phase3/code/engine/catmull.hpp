// catmull.hpp
#ifndef CATMULL_H
#define CATMULL_H

#include "../utils/point.hpp"

void getCatmullRomPoint(float t, POINT p0, POINT p1, POINT p2, POINT p3, POINT pos, POINT deriv);

void getGlobalCatmullRomPoint(float gt, std::vector<POINT> control_points, POINT pos, POINT deriv);

#endif // CATMULL_H