#include "plane.hpp"
#include "../utils/figure.hpp"
#include "../utils/point.hpp"

FIGURE generate_planeXZ(int length, int divisions) {
    FIGURE f = create_figure();
    f->type = PLANE;
    f->plane.length = length;
    f->plane.divisions = divisions;

    // Add vertices for the corners of the plane
    POINT p1 = new_point(-length / 2.0f, 0.0f, -length / 2.0f);  // bottom left
    POINT p2 = new_point(-length / 2.0f, 0.0f, length / 2.0f);   // top left
    POINT p3 = new_point(length / 2.0f, 0.0f, length / 2.0f);    // top right
    POINT p4 = new_point(length / 2.0f, 0.0f, -length / 2.0f);   // bottom right

    // Add indices for two triangles (forming a rectangle) for each subdivision
    for (int i = 0; i < divisions; ++i) {
        add_index(f, 0);            // bottom left
        add_index(f, (i + 1) % divisions + 1);  // top left
        add_index(f, i + 1);         // bottom right

        add_index(f, 0);            // bottom left
        add_index(f, (i + 2) % divisions + 1);  // top right
        add_index(f, (i + 1) % divisions + 1);  // top left
    }

    // Free the memory allocated for individual points
    free(p1);
    free(p2);
    free(p3);
    free(p4);

    return f;
}


FIGURE generate_planeXY(int length, int divisions) {
    FIGURE f = create_figure();
    f->type = PLANE;
    f->plane.length = length;
    f->plane.divisions = divisions;

    // Add vertices for the corners of the plane
    POINT p1 = new_point(-length / 2.0f, -length / 2.0f, 0.0f);  // bottom left
    POINT p2 = new_point(-length / 2.0f, length / 2.0f, 0.0f);   // top left
    POINT p3 = new_point(length / 2.0f, length / 2.0f, 0.0f);    // top right
    POINT p4 = new_point(length / 2.0f, -length / 2.0f, 0.0f);   // bottom right

    // Add indices for two triangles (forming a rectangle) for each subdivision
    for (int i = 0; i < divisions; ++i) {
        add_index(f, 0);            // bottom left
        add_index(f, (i + 1) % divisions + 1);  // top left
        add_index(f, i + 1);         // bottom right

        add_index(f, 0);            // bottom left
        add_index(f, (i + 2) % divisions + 1);  // top right
        add_index(f, (i + 1) % divisions + 1);  // top left
    }

    // Free the memory allocated for individual points
    free(p1);
    free(p2);
    free(p3);
    free(p4);

    return f;
}

FIGURE generate_planeYZ(int length, int divisions) {
    FIGURE f = create_figure();
    f->type = PLANE;
    f->plane.length = length;
    f->plane.divisions = divisions;

    // Add vertices for the corners of the plane
    POINT p1 = new_point(0.0f, -length / 2.0f, -length / 2.0f);  // bottom left
    POINT p2 = new_point(0.0f, -length / 2.0f, length / 2.0f);   // top left
    POINT p3 = new_point(0.0f, length / 2.0f, length / 2.0f);    // top right
    POINT p4 = new_point(0.0f, length / 2.0f, -length / 2.0f);   // bottom right

    // Add indices for two triangles (forming a rectangle) for each subdivision
    for (int i = 0; i < divisions; ++i) {
        add_index(f, 0);            // bottom left
        add_index(f, (i + 1) % divisions + 1);  // top left
        add_index(f, i + 1);         // bottom right

        add_index(f, 0);            // bottom left
        add_index(f, (i + 2) % divisions + 1);  // top right
        add_index(f, (i + 1) % divisions + 1);  // top left
    }

    // Free the memory allocated for individual points
    free(p1);
    free(p2);
    free(p3);
    free(p4);

    return f;
}