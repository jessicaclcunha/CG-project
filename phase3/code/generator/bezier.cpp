#include "bezier.hpp"

std::vector<std::vector<POINT>> read_patch_file(const char* patches_file) {
    std::vector<std::vector<POINT>> control_points;
    std::ifstream file(patches_file);
    if (!file.is_open()) {
        std::cerr << "Erro ao abrir o arquivo de patches." << std::endl;
        return control_points;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::vector<POINT> patch_points;
        float x, y, z;
        while (iss >> x >> y >> z) {
            POINT p = new_point(x, y, z);
            patch_points.push_back(p);
        }
        control_points.push_back(patch_points);
    }

    file.close();
    return control_points;
}

FIGURE generatePatch(float tessellation, const char* patches_file) {
    FIGURE f;

    std::vector<std::vector<POINT>> control_points = read_patch_file(patches_file);


    // Ler os patches do arquivo
    for (const auto& patch : control_points) {
        std::vector<POINT> patch_vertices;
        // Calcular vértices para o patch usando a tesselação
        for (float u = 0; u <= 1; u += tessellation) {
            for (float v = 0; v <= 1; v += tessellation) {
                POINT p = calculate_bezier_point(patch, u, v);
                patch_vertices.push_back(p);
            }
        }
        // Criar triângulos para o patch
        for (size_t i = 0; i < patch_vertices.size() - 1; ++i) {
            if ((i + 1) % (int)(1 / tessellation) != 0) {
                TRIANGLE t1 = create_triangle_with_vertices({patch_vertices[i], patch_vertices[i + 1], patch_vertices[i + 1 + (int)(1 / tessellation)]});
                TRIANGLE t2 = create_triangle_with_vertices({patch_vertices[i], patch_vertices[i + 1 + (int)(1 / tessellation)], patch_vertices[i + (int)(1 / tessellation)]});
                add_triangle(f, t1);
                add_triangle(f, t2);
            }
        }
    }

    return f;
}
