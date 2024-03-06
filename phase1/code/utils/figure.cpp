#define GL_SILENCE_DEPRECATION
#include "figure.hpp"


using namespace std;

typedef struct figure {
    FIGURE_TYPE type;
    std::vector<TRIANGLE>* triangles;
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

FIGURE create_figure_empty() {
    FIGURE f = (FIGURE)malloc(sizeof(struct figure));
    f->type = UNKNOWN;
    f->triangles = new vector<TRIANGLE>();
    return f;
}

FIGURE create_figure (FIGURE_TYPE type, int length, int divisions) {
    FIGURE f = create_figure_empty();
    f->type = type;

    switch (type) {
        case PLANE:
            f->plane.length = length;
            f->plane.divisions = divisions;
            break;
        case BOX:
            f->box.length = length;
            f->box.divisions = divisions;
            break;
        default:
            fprintf(stderr, "Erro: Tipo desconhecido\n");
            break;
    }

    return f;
}

FIGURE create_figure_plane_box (FIGURE_TYPE type, int length, int divisions) {
    FIGURE f = create_figure_empty();
    f->type = type;

    switch (type) 
    {
        case PLANE:
            f->plane.length = length;
            f->plane.divisions = divisions;
            f->triangles = new vector<TRIANGLE>();
            break;
        case BOX:
            f->box.length = length;
            f->box.divisions = divisions;
            break;
        default:
            fprintf(stderr, "Erro: Tipo desconhecido\n");
            break;
    }
    return f;
}

FIGURE create_figure_sphere (float radius, int slices, int stacks) {
    FIGURE f = create_figure_empty();
    f->type = SPHERE;
    f->sphere.radius = radius;
    f->sphere.slices = slices;
    f->sphere.stacks = stacks;
    return f;
}

FIGURE create_figure_cone (float height, float radius, int slices, int stacks) {
    FIGURE f = create_figure_empty();
    f->type = CONE;
    f->cone.height = height;
    f->cone.radius = radius;
    f->cone.slices = slices;
    f->cone.stacks = stacks;
    return f;
}

void add_triangle(FIGURE f, TRIANGLE t) {
    if (f != NULL) {
        f->triangles->push_back(t);
    }
}

void add_triangles (FIGURE f, std::vector<TRIANGLE> t) {
    if (f != NULL) {
        f->triangles->insert(f->triangles->end(), t.begin(), t.end());
    }
}

/*
void add_face(FIGURE f, POINT p1, POINT p2, POINT p3, POINT p4, int divisions) {
    // Adicionar vértices para o rosto
    add_vertex(f, p1);
    add_vertex(f, p2);
    add_vertex(f, p3);
    add_vertex(f, p4);

    // Adicionar índices para dois triângulos (formando um retângulo) para cada subdivisão
    for (i
nt i = 0; i < divisions; ++i) {
        add_index(f, 4 * i);             // Inferior esquerdo
        add_index(f, 4 * i + 1);         // Superior esquerdo
        add_index(f, 4 * i + 2);         // Superior direito

        add_index(f, 4 * i);             // Inferior esquerdo
        add_index(f, 4 * i + 2);         // Superior direito
        add_index(f, 4 * i + 3);         // Inferior direito
    }
}
*/


int number_triangles(FIGURE f){
    if (f != NULL){
        return f->triangles->size();
    }
    return 0;
}



void save_file(FIGURE f, std::string filename) {
    
    const char* filename_cstr = filename.c_str();

    FILE* file = fopen(filename_cstr, "w"); 
    if (!file) {
        fprintf(stderr, "Erro: Não foi possível abrir o arquivo %s\n", filename_cstr);
        return;
    }

    switch (f->type) {
       case BOX:
            fprintf(file, "CAIXA\nLength: %d\nDivisions: %d\nNº de Triângulos: %d\n", f->box.length, f->box.divisions, number_triangles(f));

            fprintf(file, "TRIANGULOS:\n");
            fprintf(file, "%s", print_triangulos(f).c_str());
            break;
        case CONE:
            fprintf(file, "CONE\nHeight: %.2f\nRadius: %.2f\nSlices: %d\nStacks: %d\nNº de Triângulos: %d\n",
                    f->cone.height, f->cone.radius, f->cone.slices, f->cone.stacks, number_triangles(f));

            fprintf(file, "TRIANGULOS:\n");
            fprintf(file, "%s", print_triangulos(f).c_str());
            break;
        case PLANE:
            fprintf(file, "PLANO\nLength: %d\nDivisions: %d\nNº de Triângulos: %d\n", f->plane.length, f->plane.divisions, number_triangles(f));

            fprintf(file, "\nTRIANGULOS:\n");
            fprintf(file, "%s", print_triangulos(f).c_str()); 
            break;
        case SPHERE:
            fprintf(file, "ESFERA\nRadius: %.2f\nSlices: %d\nStacks: %d\nNº de Triângulos: %d\n",
                    f->sphere.radius, f->sphere.slices, f->sphere.stacks, number_triangles(f));

            fprintf(file, "TRIANGULOS:\n");
            fprintf(file, "%s", print_triangulos(f).c_str());
            break;
        default:
            fprintf(stderr, "Erro: Tipo desconhecido\n");
            break;
    }

    fclose(file);
    free(f);
}



FIGURE file2figure(const std::string& filename) {
    FILE* file = fopen(filename.c_str(), "r");
    if (!file) {
        fprintf(stderr, "Erro: Não foi possível abrir o arquivo %s\n", filename.c_str());
        return nullptr;
    }

    FIGURE figure = create_empty_figure();

    char type[20];
    if (fscanf(file, "%s", type) == 1) {
        if (strcmp(type, "PLANO") == 0) {
            int length, divisions, numVertices;
            fscanf(file, "Length: %d\nDivisions: %d\nNº de Vertices: %d", &length, &divisions, &numVertices);

            // Processar vértices
            for (int i = 0; i < numVertices; ++i) {
                float vx, vy, vz;
                fscanf(file, "(%f, %f, %f)", &vx, &vy, &vz);
                POINT vertex = new_point(vx, vy, vz);
                add_vertex(figure, vertex);
            }

            figure->plane.length = length;
            figure->plane.divisions = divisions;
        } else if (strcmp(type, "BOX") == 0) {
            int boxLength, boxDivisions, numVertices;
            fscanf(file, "Length: %d\nDivisions: %d\nNº de Vertices: %d", &boxLength, &boxDivisions, &numVertices);

            // Processar vértices
            for (int i = 0; i < numVertices; ++i) {
                float vx, vy, vz;
                fscanf(file, "(%f, %f, %f)", &vx, &vy, &vz);
                POINT vertex = new_point(vx, vy, vz);
                add_vertex(figure, vertex);
            }

            figure->box.length = boxLength;
            figure->box.divisions = boxDivisions;
        } else if (strcmp(type, "CONE") == 0) {
            float coneHeight, coneRadius;
            int coneSlices, coneStacks, numVertices;
            fscanf(file, "Height: %f\nRadius: %f\nSlices: %d\nStacks: %d\nNº de Vertices: %d", &coneHeight, &coneRadius, &coneSlices, &coneStacks, &numVertices);

            // Processar vértices
            for (int i = 0; i < numVertices; i++) {
                float vx, vy, vz;
                fscanf(file, "(%f, %f, %f)", &vx, &vy, &vz);
                POINT vertex = new_point(vx, vy, vz);
                add_vertex(figure, vertex);
            }

            figure->cone.height = coneHeight;
            figure->cone.radius = coneRadius;
            figure->cone.slices = coneSlices;
            figure->cone.stacks = coneStacks;
        } else if (strcmp(type, "SPHERE") == 0) {
            float sphereRadius;
            int sphereSlices, sphereStacks, numVertices;
            fscanf(file, "Radius: %f\nSlices: %d\nStacks: %d\nNº de Vertices: %d", &sphereRadius, &sphereSlices, &sphereStacks, &numVertices);

            // Processar vértices
            for (int i = 0; i < numVertices; ++i) {
                float vx, vy, vz;
                fscanf(file, "(%f, %f, %f)", &vx, &vy, &vz);
                POINT vertex = new_point(vx, vy, vz);
                add_vertex(figure, vertex);
            }

            figure->sphere.radius = sphereRadius;
            figure->sphere.slices = sphereSlices;
            figure->sphere.stacks = sphereStacks;
        } else {
            fprintf(stderr, "Erro: Tipo de figura desconhecido\n");
            free_figure(figure);
        }
    }

    fclose(file);
    return figure;
}






/*bool comparePoints(POINT a, POINT b) {
    if (get_X(a)!= get_X(b)) return get_X(a) < get_X(b);
    if (get_Y(a) != get_Y(b)) return get_Y(a) <get_Y(b);
    return get_Z(a) < get_Z(b);
}

void concatenateAndRemoveDuplicates(FIGURE f1, FIGURE f2) {
    // Concatenar listas de vértices
    f1->vertices->insert(f1->vertices->end(), f2->vertices->begin(), f2->vertices->end());

    // Ordenar vértices para preparar para a remoção de duplicatas
    std::sort(f1->vertices->begin(), f1->vertices->end(), comparePoints);
    // Remover duplicatas
    auto last = std::unique(f1->vertices->begin(), f1->vertices->end());
    
    f1->vertices->erase(last, f1->vertices->end());


}*/

void print_figura(FIGURE f) {
    if (f != NULL) {
        if (f->type == PLANE) {
            printf("Vertices:\n");
            for (const TRIANGLE &triangulo : *(f->triangles)) {
                std::vector<POINT>* vertexBegin = get_points(triangulo);
                for (const POINT &vertex : *vertexBegin) {
                    printf("(%.2f, %.2f, %.2f)\n", get_X(vertex), get_Y(vertex), get_Z(vertex));
                }
            }
        }
        if (f->type == BOX){
            printf("Vertices:\n");
            for (const TRIANGLE &triangulo : *(f->triangles)) {
                std::vector<POINT>* vertexBegin = get_points(triangulo);
                for (const POINT &vertex : *vertexBegin) {
                    printf("(%.2f, %.2f, %.2f)\n", get_X(vertex), get_Y(vertex), get_Z(vertex));
                }
            }
        }
        if (f->type == SPHERE){
            printf("Vertices:\n");
            for (const TRIANGLE &triangulo : *(f->triangles)) {
                std::vector<POINT>* vertexBegin = get_points(triangulo);
                for (const POINT &vertex : *vertexBegin) {
                    printf("(%.2f, %.2f, %.2f)\n", get_X(vertex), get_Y(vertex), get_Z(vertex));
                }
            }
        }
        if (f->type == CONE){
            printf("Vertices:\n");
            for (const TRIANGLE &triangulo : *(f->triangles)) {
                std::vector<POINT>* vertexBegin = get_points(triangulo);
                for (const POINT &vertex : *vertexBegin) {
                    printf("(%.2f, %.2f, %.2f)\n", get_X(vertex), get_Y(vertex), get_Z(vertex));
                }
            }
        }
    }
}


std::string print_triangulos(FIGURE f) {
    std::string output;
    if (!f) {
        output = "Error: Invalid figure.";
        return output;
    }

    std::vector<TRIANGLE>* triangles = f->triangles;

    if (!triangles) {
        output = "Error: No triangles in the figure.";
        return output;
    }

    for (const TRIANGLE &triangulo : *triangles) {
        std::vector<POINT>* vertices = get_points(triangulo);
        if (!vertices || vertices->size() != 3) {
            output += "Error: Invalid triangle - should have exactly 3 vertices.\n";
            continue;
        }
        output += "[";
        for (size_t j = 0; j < vertices->size(); ++j) {
            const POINT& point = (*vertices)[j];
            char buffer[100]; // Buffer to store point coordinates
            snprintf(buffer, sizeof(buffer), "(%.2f, %.2f, %.2f)", get_X(point), get_Y(point), get_Z(point));
            output += buffer;
            if (j < vertices->size() - 1) output += ",";
        }
        output += "]\n";
    }

    return output;
}


