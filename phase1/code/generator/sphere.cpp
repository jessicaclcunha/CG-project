#include "sphere.hpp"
#include <vector>
#include <cmath>


FIGURE generate_sphere(float radius, int slices, int stacks) {
    FIGURE f = create_figure(SPHERE, length, divisions);
    std::vector<POINT> vertices;
    std::vector<int> indices;
    for (int stack = 0; stack <= stacks; ++stack) {
        float phi = M_PI * float(stack) / float(stacks); // [0, PI]

        for (int slice = 0; slice <= slices; ++slice) {
            float theta = 2.0 * M_PI * float(slice) / float(slices); // [0, 2PI]

            Point vertex;
            vertex.x = radius * sin(phi) * cos(theta);
            vertex.y = radius * cos(phi);
            vertex.z = radius * sin(phi) * sin(theta);

            vertices.push_back(vertex);
        }
    }

    // Generate indices
    for (int stack = 0; stack < stacks; ++stack) {
        for (int slice = 0; slice < slices; ++slice) {
            int first = (stack * (slices + 1)) + slice;
            int second = first + slices + 1;

            indices.push_back(first);
            indices.push_back(second);
            indices.push_back(first + 1);

            indices.push_back(second);
            indices.push_back(second + 1);
            indices.push_back(first + 1);
        }
    }
    add_vertexs(f, vertices);
    add_indexs(f, indices);
};