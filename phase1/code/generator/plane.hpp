// plane.hpp
#ifndef PLANE_H
#define PLANE_H

#include "../utils/figure.hpp"
#include "../utils/point.hpp"
#include <stdio.h>
#include <vector>
#include <memory>

FIGURE generate_plane(int length, int divisions, float x, float y, float z, float dx, float dy, float dz);

FIGURE generate_planeXZ(int length, int divisions);

FIGURE generate_planeXY(int length, int divisions);

FIGURE generate_planeYZ(int length, int divisions);


#endif // PLANE_H