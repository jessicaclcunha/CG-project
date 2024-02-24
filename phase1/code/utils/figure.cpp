#include "ponto.hpp"

typedef enum {
    BOX,
    CONE,
    PLANE,
    SPHERE
} FIGURE_TYPE;

typedef struct figure {
    FIGURE_TYPE type;

    union {
        struct {
            PONTO *centro;
            float raio;
        } sphere;

        struct {
            PONTO *base;
            float altura;
            float raio;
        } cone;

        struct {
            PONTO *ponto1;
            PONTO *ponto2;
            PONTO *ponto3;
        } box;

        struct {
            PONTO *ponto1;
            PONTO *ponto2;
            PONTO *ponto3;
            PONTO *ponto4;
        } plane;
    };
} FIGURE;