// figure.hpp
#include <string>
#include "point.hpp"
#ifndef FIGURE_H
#define FIGURE_H

enum FIGURE_TYPE {
    BOX,
    CONE,
    PLANE,
    SPHERE
};


typedef struct figure *FIGURE;

FIGURE create_figure();

void add_vertex(FIGURE f, POINT p);

void add_index(FIGURE f, int index);


void add_face(FIGURE f, POINT p1, POINT p2, POINT p3, POINT p4, int divisions);
void save_file(const FIGURE f, std::string filename);

#endif // FIGURE_H