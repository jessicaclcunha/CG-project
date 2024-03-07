#include "sphere.hpp"

FIGURE generate_sphere(float radius, int slices, int stacks) {
    FIGURE sphere = create_figure_sphere(radius, slices, stacks);

    for (int i = 0; i < stacks; ++i) {
        for (int j = 0; j < slices; ++j) {
            float phi = 2 * M_PI * j / slices;
            float theta = M_PI * (i + 1) / (stacks + 1);
            float nextTheta = M_PI * (i + 2) / (stacks + 1);

            POINT p1, p2, p3, p4;
            p1 = new_point( radius * sin(theta) * cos(phi),
            radius * sin(theta) * sin(phi),
            radius * cos(theta));

            p2 = new_point(radius * sin(theta) * cos(phi + 2 * M_PI / slices),
            radius * sin(theta) * sin(phi + 2 * M_PI / slices),
            radius * cos(theta));

            p3 = new_point (radius * sin(nextTheta) * cos(phi),
            radius * sin(nextTheta) * sin(phi),
            radius * cos(nextTheta));

            p4 = new_point (radius * sin(nextTheta) * cos(phi + 2 * M_PI / slices),
            radius * sin(nextTheta) * sin(phi + 2 * M_PI / slices),
            radius * cos(nextTheta));

            std::vector<POINT> vertices1;
            std::vector<POINT> vertices2; 

            vertices1.push_back(p1); //adiciona o ponto 1 à lista de vertices
            vertices1.push_back(p3); //...
            vertices1.push_back(p2); //...
            vertices2.push_back(p2); //...
            vertices2.push_back(p3); //...
            vertices2.push_back(p4); //...

            TRIANGLE t1 = create_triangle_with_vertices(vertices1);
            TRIANGLE t2 = create_triangle_with_vertices(vertices2);

            add_triangle(sphere, t1);
            add_triangle(sphere, t2);

        }
    }

    return sphere;
}