#include "color.hpp"

COLOR new_color(COLOR_TYPE type, float r, float g, float b, float value) {
    COLOR c;
    c.type = type;
    switch (type) {
        case DIFFUSE:
            c.diffuse.r = r;
            c.diffuse.g = g;
            c.diffuse.b = b;
            break;
        case AMBIENT:
            c.ambient.r = r;
            c.ambient.g = g;
            c.ambient.b = b;
            break;
        case SPECULAR:
            c.specular.r = r;
            c.specular.g = g;
            c.specular.b = b;
            break;
        case EMISSIVE:
            c.emissive.r = r;
            c.emissive.g = g;
            c.emissive.b = b;
            break;
        case SHININESS:
            c.shininess.value = value;
            break;
    }
    return c;
}

