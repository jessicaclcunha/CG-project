#include "plane.hpp"
#include "../utils/figure.hpp"
#include "../utils/point.hpp"

FIGURE generate_plane(int length, int divisions, float x, float y, float z, float dx, float dy, float dz) {
    FIGURE f = create_figure();
    f->type = PLANE;
    f->plane.length = length;
    f->plane.divisions = divisions;

    // Add vertices for the corners of the plane
    POINT p1 = new_point(x, y, z);
    POINT p2 = new_point(x + dx, y + dy, z + dz);
    POINT p3 = new_point(x + 2 * dx, y + 2 * dy, z + 2 * dz);
    POINT p4 = new_point(x + 3 * dx, y + 3 * dy, z + 3 * dz);

    // Add indices for two triangles (forming a rectangle) for each subdivision
    for (int i = 0; i < divisions; ++i) {
        add_index(f, 0);
        add_index(f, (i + 1) % divisions + 1);
        add_index(f, i + 1);

        add_index(f, 0);
        add_index(f, (i + 2) % divisions + 1);
        add_index(f, (i + 1) % divisions + 1);
    }

    return f;
}

FIGURE generate_planeXZ(int length, int divisions) {
    return generate_plane(length, divisions, -length / 2.0f, 0.0f, -length / 2.0f, length / divisions, 0.0f, length / divisions);
}

FIGURE generate_planeXY(int length, int divisions) {
    return generate_plane(length, divisions, -length / 2.0f, -length / 2.0f, 0.0f, length / divisions, length / divisions, 0.0f);
}

FIGURE generate_planeYZ(int length, int divisions) {
    return generate_plane(length, divisions, 0.0f, -length / 2.0f, -length / 2.0f, 0.0f, length / divisions, length / divisions);
}
