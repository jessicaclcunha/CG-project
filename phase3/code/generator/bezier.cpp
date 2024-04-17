#include "bezier.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

std::vector<std::vector<POINT> > parse_patch_file(const char* filePath) {
    std::ifstream file(filePath);
    std::vector<std::vector<POINT> > result;

    if (file.is_open()) {
        std::string line;
        // Obtenção do número de patches
        if (!std::getline(file, line)) return result;
        int numPatches = std::stoi(line);
        std::vector<std::vector<int> > indicesPerPatch;

        // Obtenção dos índices de cada patch
        for (int i = 0; i < numPatches; i++) {
            if (!std::getline(file, line)) return result;
            std::istringstream iss(line);
            std::vector<int> indices;
            int index;
            while (iss >> index) {
                indices.push_back(index);
                if (iss.peek() == ',') iss.ignore();
            }
            indicesPerPatch.push_back(indices);
        }

        // Obtenção do número de pontos de controlo
        if (!std::getline(file, line)) return result;
        int numControlPoints = std::stoi(line);

        // Obtenção dos pontos de controlo
        std::vector<POINT> controlPoints;
        for (int i = 0; i < numControlPoints; i++) {
            if (!std::getline(file, line)) return result;
            std::istringstream iss(line);
            float x, y, z;
            char comma;
            if (!(iss >> x >> comma >> y >> comma >> z)) 
                return result;
            POINT point = new_point(x, y, z);
            controlPoints.push_back(point);
        }

        // Construção dos patches
        for (std::vector<int> indices : indicesPerPatch) {
            std::vector<POINT> patch;
            for (int indice : indices) {
                patch.push_back(controlPoints[indice]);
            }
            result.push_back(patch);
        }
        file.close();
    }
    printf("Parsed %lu patches\n", result.size());
    return result;
}

POINT bezier_interpolate_row(std::vector<POINT> controlPoints, float t) {
    if (controlPoints.size() < 2) {
        // Não há pontos suficientes para interpolação
        // Você pode querer lidar com isso de forma diferente
        return POINT(); // Ponto vazio
    }

    std::vector<POINT> points;
    for (int i = 0; i < controlPoints.size() - 1; i++) {
        float x = (1 - t) * get_X(controlPoints[i]) + t * get_X(controlPoints[i + 1]);
        float y = (1 - t) * get_Y(controlPoints[i]) + t * get_Y(controlPoints[i + 1]);
        float z = (1 - t) * get_Z(controlPoints[i]) + t * get_Z(controlPoints[i + 1]);
        POINT p = new_point(x, y, z);
        points.push_back(p);
    }
    if (points.size() == 1) {
        return points[0]; // Se só houver um ponto, retornar esse ponto
    }
    // Se houver mais de um ponto, continuar interpolação
    return bezier_interpolate_row(points, t);
}



POINT bezier_interpolate(std::vector<POINT> patch, float u, float v) {
    std::vector<POINT> points;
    for (int i = 0; i < 4; i++) {
        std::vector<POINT> row;
        for (int j = 0; j < 4; j++) {
            row.push_back(patch[i * 4 + j]);
        }
        points.push_back(bezier_interpolate_row(row, u));
    }
    return bezier_interpolate_row(points, v);
}


std::vector<TRIANGLE> generate_bezier_surfaces(std::vector<POINT> patch, float tessellation) {
    std::vector<TRIANGLE> triangles;
    for (int i = 0; i < tessellation; i++) {
        for (int j = 0; j < tessellation; j++) {
            float u = static_cast<float>(i) / tessellation;
            float v = static_cast<float>(j) / tessellation;


            POINT p1 = bezier_interpolate(patch, u, v);
            POINT p2 = bezier_interpolate(patch, u + 1.0f / tessellation, v);
            POINT p3 = bezier_interpolate(patch, u, v + 1.0f / tessellation);
            POINT p4 = bezier_interpolate(patch, u + 1.0f / tessellation, v + 1.0f / tessellation);


            TRIANGLE t1 = create_triangle();
            add_vertex(t1, p1);
            add_vertex(t1, p2);
            add_vertex(t1, p3);

            TRIANGLE t2 = create_triangle();
            add_vertex(t2, p2);
            add_vertex(t2, p3);
            add_vertex(t2, p4);

            triangles.push_back(t1);
            triangles.push_back(t2);
        }
    }
    return triangles;
}

FIGURE generate_bezier(float tessellation, const char* patches_file) {
    FIGURE figure = create_figure_bezier(tessellation, patches_file);
    std::vector<std::vector<POINT> > patches = parse_patch_file(patches_file);
    for (std::vector<POINT> patch : patches) 
    {
        std::vector<TRIANGLE> triangles = generate_bezier_surfaces(patch, tessellation);
        for (TRIANGLE t : triangles)
        {
            add_triangle(figure, t);
        }
    }
    return figure;
}
