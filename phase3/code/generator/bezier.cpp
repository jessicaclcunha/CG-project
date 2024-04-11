#include "bezier.hpp"
/*

std::vector<TRIANGLE> generate_bezier_surfaces(std::vector<POINT> patch_control_points, float tessellation) {
    std::vector<TRIANGLE> surface_triangles;

    // Calculate vertices for the patch using tessellation
    std::vector<std::vector<POINT>> patch_vertices;
    for (float u = 0; u <= 1; u += tessellation) {
        std::vector<POINT> row_vertices;
        for (float v = 0; v <= 1; v += tessellation) {
            POINT p = calculate_bezier_point(patch_control_points, u, v);
            row_vertices.push_back(p);
        }
        patch_vertices.push_back(row_vertices);
    }

    // Create triangles for the patch
    for (size_t i = 0; i < patch_vertices.size() - 1; ++i) {
        for (size_t j = 0; j < patch_vertices[i].size() - 1; ++j) {
            TRIANGLE t1 = create_triangle_with_vertices({patch_vertices[i][j], patch_vertices[i][j + 1], patch_vertices[i + 1][j]});
            TRIANGLE t2 = create_triangle_with_vertices({patch_vertices[i][j + 1], patch_vertices[i + 1][j + 1], patch_vertices[i + 1][j]});
            surface_triangles.push_back(t1);
            surface_triangles.push_back(t2);
        }
    }
    return surface_triangles;
}

FIGURE generate_patch(float tessellation, const char* patches_file) {
    std::ifstream file(patches_file);
    if (!file.is_open()) {
        std::cerr << "Error: Failed to open patches file." << std::endl;
        return nullptr;
    }

    int num_patches;
    file >> num_patches;
    if (num_patches <= 0) {
        std::cerr << "Error: Invalid number of patches." << std::endl;
        return nullptr;
    }

    std::vector<std::vector<POINT>> control_points; // Vector to store control points for each patch

    // Read control points for each patch
    for (int i = 0; i < num_patches; ++i) {
        int num_control_points;
        file >> num_control_points;
        if (num_control_points != 16) {
            std::cerr << "Error: Invalid number of control points for patch " << i + 1 << std::endl;
            return nullptr;
        }

        std::vector<POINT> patch_control_points;
        for (int j = 0; j < num_control_points; ++j) {
            float x, y, z;
            file >> x >> y >> z;
            patch_control_points.push_back(new_point(x, y, z));
        }

        control_points.push_back(patch_control_points);
    }

    file.close();

    // Generate Bezier surfaces for each patch
    FIGURE figure = create_figure_bezier(tessellation, patches_file);
    for (const auto& patch_control_points : control_points) {
        std::vector<TRIANGLE> surface_triangles = generate_bezier_surfaces(patch_control_points, tessellation);
        add_triangles(figure, surface_triangles);
    }
    return figure;
}
*/