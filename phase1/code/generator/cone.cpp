#include "cone.hpp"

FIGURE generate_cone(float radius, float height, int slices, int stacks) {
    FIGURE f = create_figure(CONE, radius, height);
    std::vector<POINT> vertices;
    std::vector<int> indices;
    // Gera vértices e índices para o cone
    for (int stack = 0; stack <= stacks; ++stack) {
        float stackHeight = height * stack / stacks;
        float stackRadius = radius * (1 - (float)stack / stacks);

        for (int slice = 0; slice <= slices; ++slice) {
            float theta = 2.0f * M_PI * slice / slices;
            float x = stackRadius * cos(theta);
            float y = stackHeight;
            float z = stackRadius * sin(theta);

            vertices.push_back({ x, y, z });
        }
    }

    // Vértice superior do cone
    vertices.push_back({ 0, height, 0 });

    // Gera índices para as faces laterais
    int topIndex = vertices.size() - 1;
    for (int stack = 0; stack < stacks; ++stack) {
        for (int slice = 0; slice < slices; ++slice) {
            int first = (stack * (slices + 1)) + slice;
            int second = first + slices + 1;

            indices.push_back(first);
            indices.push_back(second);
            indices.push_back(first + 1);

            // Triângulo para o topo
            if (stack == stacks - 1) {
                indices.push_back(topIndex);
                indices.push_back(first + 1);
                indices.push_back(second);
            }
        }
    }

    // Gera índices para a base
    vertices.push_back({ 0, 0, 0 }); // Centro da base
    int baseCenterIndex = vertices.size() - 1;
    for (int slice = 0; slice < slices; ++slice) {
        indices.push_back(baseCenterIndex);
        indices.push_back(slice + 1);
        indices.push_back(slice);
    }
    add_vertexs(f, vertices);
    add_indexs(f, indices);
}
