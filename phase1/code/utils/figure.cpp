#include "point.hpp"
#include <stdio.h>

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

        struct {
            int length;
            int divisions;
        } plane;

        struct {
            int length;
            int divisions;
        } box;
    };
} *FIGURE;

FIGURE create_figure() {
    FIGURE f = (FIGURE) malloc(sizeof(struct figure));
    return f;
}

void save_file(const FIGURE f, const char* filename) {
    FILE* file = fopen(filename, "w");

    if (!file) {
        fprintf(stderr, "Erro: Não foi possível abrir o arquivo %s\n", filename);
        return;
    }

    switch (f->type) {
        case BOX:
            fprintf(file, "CAIXA\nLength: %d\nDivisions: %d\n", f->box.length, f->box.divisions);
            break;
        case CONE:
            fprintf(file, "CONE\nHeight: %.2f\nRadius: %.2f\nSlices: %d\nStacks: %d\n",
                    f->cone.height, f->cone.radius, f->cone.slices, f->cone.stacks);
            break;
        case PLANE:
            fprintf(file, "PLANO\nLength: %d\nDivisions: %d\n", f->plane.length, f->plane.divisions);
            break;
        case SPHERE:
            fprintf(file, "ESFERA\nRadius: %.2f\nSlices: %d\nStacks: %d\n",
                    f->sphere.radius, f->sphere.slices, f->sphere.stacks);
            break;
        default:
            fprintf(stderr, "Erro: Tipo desconhecido\n");
            break;
    }

    fclose(file);
    free(f); 
}