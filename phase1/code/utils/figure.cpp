#define GL_SILENCE_DEPRECATION
#include "figure.hpp"

using namespace std;


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

FIGURE create_figure() {
    FIGURE f = (FIGURE)malloc(sizeof(struct figure));
    f->plane.vertices = new vector<POINT>();
    f->plane.indices = new vector<int>();
    return f;
}

void add_vertex(FIGURE f, POINT p) {
    if (f != NULL) {
        if (f->type == PLANE) {
            f->plane.vertices->push_back(p);
        }
        // Adicionar verificações semelhantes para outros tipos de figura, se necessário
    }
}

void add_index(FIGURE f, int index) {
    if (f != NULL) {
        if (f->type == PLANE) {
            f->plane.indices->push_back(index);
        }
        // Adicionar verificações semelhantes para outros tipos de figura, se necessário
    }
}

void add_face(FIGURE f, POINT p1, POINT p2, POINT p3, POINT p4, int divisions) {
    // Adicionar vértices para o rosto
    add_vertex(f, p1);
    add_vertex(f, p2);
    add_vertex(f, p3);
    add_vertex(f, p4);

    // Adicionar índices para dois triângulos (formando um retângulo) para cada subdivisão
    for (int i = 0; i < divisions; ++i) {
        add_index(f, 4 * i);             // Inferior esquerdo
        add_index(f, 4 * i + 1);         // Superior esquerdo
        add_index(f, 4 * i + 2);         // Superior direito

        add_index(f, 4 * i);             // Inferior esquerdo
        add_index(f, 4 * i + 2);         // Superior direito
        add_index(f, 4 * i + 3);         // Inferior direito
    }
}

void save_file(FIGURE f, const char* filename) {
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

            // Imprimir informações sobre os vértices
            fprintf(file, "Vertices:\n");
            for (const POINT vertex : *(f->plane.vertices)) {
                fprintf(file, "(%.2f, %.2f, %.2f)\n", get_X(vertex), get_Y(vertex), get_Z(vertex));
            }

            // Imprimir informações sobre os índices
            fprintf(file, "Indices:\n");
            for (int index : *(f->plane.indices)) {
                fprintf(file, "%d\n", index);
            }

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