// lights.hpp
#ifndef LIGHTS_H
#define LIGHTS_H

enum LIGHT_TYPE {
    L_POINT,
    L_DIRECTIONAL,
    L_SPOTLIGHT
};

typedef struct light {
    LIGHT_TYPE type;
    union {
        struct {
            float posX;
            float posY;
            float posZ;
        } l_point;
        struct {
            float dirX;
            float dirY;
            float dirZ;
        } l_directional;
        struct {
            float posX;
            float posY;
            float posZ;
            float dirX;
            float dirY;
            float dirZ;
            float cutoff;
        } l_spotlight;  
    };
} LIGHT;

#endif // LIGHTS_H