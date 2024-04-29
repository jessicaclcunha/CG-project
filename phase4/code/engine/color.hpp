// color.hpp
#ifndef COLOR_H
#define COLOR_H

enum COLOR_TYPE {
    DIFFUSE,
    AMBIENT,
    SPECULAR,
    EMISSIVE,
    SHININESS
};

typedef struct color {
    COLOR_TYPE type;
    union {
        struct {
            float r;
            float g;
            float b;
        } diffuse;
        struct {
            float r;
            float g;
            float b;
        } ambient; 
        struct {
            float r;
            float g;
            float b;
        } specular; 
        struct {
            float r;
            float g;
            float b;
        } emissive; 
        struct {
            float value;
        } shininess; 
    };
} COLOR;

#endif //   COLOR_H