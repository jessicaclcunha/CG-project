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

FIGURE create_figure_empty() {
    FIGURE f = (FIGURE)malloc(sizeof(struct figure));
    f->plane.vertices = new vector<POINT>();
    f->plane.indices = new vector<int>();
    return f;
}

FIGURE create_figure (FIGURE_TYPE type, int length, int divisions) {
    FIGURE f = create_figure_empty();
    f->type = type;

    /*switch (type) {
        case SPHERE:
            f->sphere.radius = length;
            f->sphere.slices = divisions;
            f->sphere.stacks = divisions;
            break;
        case CONE:
            f->cone.height = length;
            f->cone.radius = divisions;
            f->cone.slices = divisions;
            f->cone.stacks = divisions;
            break;
        case PLANE:*/
            f->plane.length = length;
            f->plane.divisions = divisions;
           /* break;
        case BOX:
            f->box.length = length;
            f->box.divisions = divisions;
            break;
        default:
            fprintf(stderr, "Erro: Tipo desconhecido\n");
            break;
    }*/

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

void save_file(FIGURE f, std::string filename) {
    // Obter a representação C-style da string do filename
    const char* filename_cstr = filename.c_str();

    FILE* file = fopen(filename_cstr, "w"); // Use a representação C-style

    if (!file) {
        fprintf(stderr, "Erro: Não foi possível abrir o arquivo %s\n", filename_cstr);
        return;
    }

    switch (f->type) {
       /*case BOX:
            fprintf(file, "CAIXA\nLength: %d\nDivisions: %d\n", f->box.length, f->box.divisions);
            break;
        case CONE:
            fprintf(file, "CONE\nHeight: %.2f\nRadius: %.2f\nSlices: %d\nStacks: %d\n",
                    f->cone.height, f->cone.radius, f->cone.slices, f->cone.stacks);
            break;*/
        case PLANE:
            fprintf(file, "PLANO\nLength: %d\nDivisions: %d\n", f->plane.length, f->plane.divisions);

            // Imprimir informações sobre os vértices
            fprintf(file, "Vertices:\n");
            for (const POINT vertex : *(f->plane.vertices)) {
                fprintf(file, "(%.2f, %.2f, %.2f)\n", get_X(vertex), get_Y(vertex), get_Z(vertex));
            }
            break;
        /*case SPHERE:
            fprintf(file, "ESFERA\nRadius: %.2f\nSlices: %d\nStacks: %d\n",
                    f->sphere.radius, f->sphere.slices, f->sphere.stacks);
            break;*/
        default:
            fprintf(stderr, "Erro: Tipo desconhecido\n");
            break;
    }

    fclose(file);
    free(f);
}

void print_vertex(POINT p) {
    printf("(%.2f, %.2f, %.2f)\n", get_X(p), get_Y(p), get_Z(p));
}

void print_index(int index) {
    printf("%d\n", index);
}

void print_all_vertices(FIGURE f) {
    if (f != NULL) {
        if (f->type == PLANE) {
            for (const POINT vertex : *(f->plane.vertices)) {
                print_vertex(vertex);
            }
        }
    }
}

void print_all_indices(FIGURE f) {
    if (f != NULL) {
        if (f->type == PLANE) {
            for (int index : *(f->plane.indices)) {
                print_index(index);
            }
        }
    }
}
