// figure.hpp
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

void save_file(const FIGURE f, std::string filename);

#endif // FIGURE_H