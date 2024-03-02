// figure.hpp
#include <string>
#include <stdio.h>
#include <vector>
#include <memory>
#include "point.hpp"
#ifndef FIGURE_H
#define FIGURE_H

using namespace std;

enum FIGURE_TYPE {
    BOX,
    CONE,
    PLANE,
    SPHERE
};

typedef struct figure {
    FIGURE_TYPE type;
    union {
        struct {
            float radius;
            int slices;
            int stacks;
        } sphere;

        struct {
            float height;
            float radius;
            int slices;
            int stacks;
        } cone;

        struct  {
            int length;
            int divisions;
            vector<POINT>* vertices;
            vector<int>* indices;
        } plane;

        struct {
            int length;
            int divisions;
        } box;
    };
} *FIGURE;

FIGURE create_figure();

void add_vertex(FIGURE f, POINT p);

void add_index(FIGURE f, int index);

void add_face(FIGURE f, POINT p1, POINT p2, POINT p3, POINT p4, int divisions);
void save_file(const FIGURE f, std::string filename);

#endif // FIGURE_H