#include "lights.hpp"

LIGHT new_light(LIGHT_TYPE type, float posX, float posY, float posZ, float dirX, float dirY, float dirZ, float cutoff) {
    LIGHT l;
    l.type = type;
    switch (type) {
        case L_POINT:
            l.l_point.posX = posX;
            l.l_point.posY = posY;
            l.l_point.posZ = posZ;
            break;
        case L_DIRECTIONAL:
            l.l_directional.dirX = dirX;
            l.l_directional.dirY = dirY;
            l.l_directional.dirZ = dirZ;
            break;
        case L_SPOTLIGHT:
            l.l_spotlight.posX = posX;
            l.l_spotlight.posY = posY;
            l.l_spotlight.posZ = posZ;
            l.l_spotlight.dirX = dirX;
            l.l_spotlight.dirY = dirY;
            l.l_spotlight.dirZ = dirZ;
            l.l_spotlight.cutoff = cutoff;
            break;
    }
    return l;
}