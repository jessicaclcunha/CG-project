// bezier.hpp
#ifndef BEZIER_H
#define BEZIER_H

#include "../utils/figure.hpp"
#include "../utils/point.hpp"

std::vector<TRIANGLE> generate_bezier_surfaces(std::vector<POINT> patch_control_points, float tessellation);

FIGURE generate_patch(float tessellation, const char* patches_file);

#endif // BEZIER_H