#include "box.hpp"
#include "../utils/figure.hpp"

FIGURE generate_box(float length, int divisions) {
    FIGURE f = create_figure();
    f->type = BOX;
    f->box.length = length;
    f->box.divisions = divisions;

    float halfLength = length / 2.0f;

    // Define the vertices of the box
    POINT p1 = new_point(-halfLength, -halfLength, -halfLength);  // Front bottom left
    POINT p2 = new_point(-halfLength, halfLength, -halfLength);   // Front top left
    POINT p3 = new_point(halfLength, halfLength, -halfLength);    // Front top right
    POINT p4 = new_point(halfLength, -halfLength, -halfLength);   // Front bottom right
    POINT p5 = new_point(-halfLength, -halfLength, halfLength);   // Back bottom left
    POINT p6 = new_point(-halfLength, halfLength, halfLength);    // Back top left
    POINT p7 = new_point(halfLength, halfLength, halfLength);     // Back top right
    POINT p8 = new_point(halfLength, -halfLength, halfLength);    // Back bottom right

    // Add vertices and indices for each face
    add_face(f, p1, p2, p3, p4, divisions);  // Front face
    add_face(f, p5, p6, p7, p8, divisions);  // Back face
    add_face(f, p1, p2, p6, p5, divisions);  // Left face
    add_face(f, p4, p3, p7, p8, divisions);  // Right face
    add_face(f, p1, p4, p8, p5, divisions);  // Bottom face
    add_face(f, p2, p3, p7, p6, divisions);  // Top face

    // Free the memory allocated for individual points
    free(p1);
    free(p2);
    free(p3);
    free(p4);
    free(p5);
    free(p6);
    free(p7);
    free(p8);

    return f;
}