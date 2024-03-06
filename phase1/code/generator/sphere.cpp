#include "sphere.hpp"

FIGURE generate_sphere(float radius, int slices, int stacks) {
    FIGURE sphere = create_figure_sphere(radius, slices, stacks);

    for (int i = 0; i < stacks; ++i) {
        for (int j = 0; j < slices-1; ++j) {
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

            // Adiciona os pontos do triângulo superior
            add_vertex (sphere, p1);
            add_vertex (sphere, p3);
            add_vertex (sphere, p2);

            // Adiciona os pontos do triângulo inferior
            add_vertex (sphere, p2);
            add_vertex (sphere, p3);
            add_vertex (sphere, p4);
        }
    }

    return sphere;
}