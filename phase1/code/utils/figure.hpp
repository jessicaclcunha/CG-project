#ifndef FIGURE_HPP
#define FIGURE_HPP


#include <stdio.h>
#include <vector>
#include <memory>
#include "point.hpp" // Inclua o arquivo de cabeçalho do ponto aqui

// Definição completa do enum FIGURE_TYPE
enum FIGURE_TYPE {
    SPHERE,
    CONE,
    PLANE,
    BOX
};

typedef struct figure *FIGURE;

FIGURE create_figure();
void add_vertex(FIGURE f, POINT p);
void add_index(FIGURE f, int index);
void add_face(FIGURE f, POINT p1, POINT p2, POINT p3, POINT p4, int divisions);
void save_file(FIGURE f, const char* filename);

#endif /* FIGURE_HPP */
