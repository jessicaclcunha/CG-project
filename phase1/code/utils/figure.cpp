#include "point.hpp"
#include <stdio.h>
#include <vector>

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

        struct {
            int length;
            int divisions;
            std::vector<POINT> vertices;
            std::vector<int> indices;
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

void add_vertex(FIGURE f, POINT p) {
    if (f != NULL) {
        // Assuming type is PLANE
        if (f->type == PLANE) {
            f->plane.vertices.push_back(p);
        }
        // Add similar checks for other figure types if needed
    }
}

void add_index(FIGURE f, int index) {
    if (f != NULL) {
        // Assuming type is PLANE
        if (f->type == PLANE) {
            f->plane.indices.push_back(index);
        }
        // Add similar checks for other figure types if needed
    }
}

void add_face(FIGURE f, POINT p1, POINT p2, POINT p3, POINT p4, int divisions) {
    // Add vertices for the face
    add_vertex(f, p1);
    add_vertex(f, p2);
    add_vertex(f, p3);
    add_vertex(f, p4);

    // Add indices for two triangles (forming a rectangle) for each subdivision
    for (int i = 0; i < divisions; ++i) {
        add_index(f, 4 * i);             // Bottom left
        add_index(f, 4 * i + 1);         // Top left
        add_index(f, 4 * i + 2);         // Top right

        add_index(f, 4 * i);             // Bottom left
        add_index(f, 4 * i + 2);         // Top right
        add_index(f, 4 * i + 3);         // Bottom right
    }
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