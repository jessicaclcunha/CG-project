#ifndef FIGURE_HPP
#define FIGURE_HPP


#include <stdio.h>
#include <vector>
#include <memory>
#include <string>
#include <cstdio>
#include "point.hpp" // Inclua o arquivo de cabeçalho do ponto aqui

// Definição completa do enum FIGURE_TYPE
enum FIGURE_TYPE {
    SPHERE,
    CONE,
    PLANE,
    BOX
};

typedef struct figure *FIGURE;

FIGURE create_figure_empty();
FIGURE create_figure(FIGURE_TYPE type, int length, int divisions);
void add_vertex(FIGURE f, POINT p);
void add_index(FIGURE f, int index);
void add_face(FIGURE f, POINT p1, POINT p2, POINT p3, POINT p4, int divisions);
void save_file(FIGURE f, std::string filename);

#endif /* FIGURE_HPP */
