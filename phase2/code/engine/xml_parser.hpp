// xml_parser.hpp
#ifndef XML_PARSER_H
#define XML_PARSER_H

#include <iostream>
#include <vector>
#include <string>
#include "../tinyxml/tinyxml.h"

enum TRANSFORM_TYPE {
    TRANSLATE,
    ROTATE,
    SCALE
};

typedef struct Camera {
    float position[3];
    float lookAt[3];
    float up[3];
    struct Projection {
        float fov;
        float near;
        float far;
    } projection;
}CAMERA;

typedef struct Model {
    std::string file;
} MODEL;

typedef struct Transform {
    TRANSFORM_TYPE type;
    union {
        struct {
            float x;
            float y;
            float z;
        } translate;
        struct {
            float angle;
            float x;
            float y;
            float z;
        } rotate;
        struct {
            float x;
            float y;
            float z;
        } scale;
    };
} TRANSFORM;

typedef struct Group GROUP;

typedef struct Group {
    std::vector<MODEL> models;
    std::vector<TRANSFORM> transforms;
    std::vector<GROUP> children;
} GROUP;

typedef struct world {
    int windowWidth;
    int windowHeight;
    Camera camera;
    std::vector<GROUP> groups;
} WORLD;

WORLD create_world ();

CAMERA create_new_camera();

MODEL create_new_model();

GROUP create_new_group();

int get_windowWidth(WORLD w);

int get_windowHeight(WORLD w);

float get_position_camX(WORLD w);

void set_position_camX(WORLD w, float x);

float get_position_camY(WORLD w);

void set_position_camY(WORLD w, float y);

float get_position_camZ(WORLD w);

void set_position_camZ(WORLD w, float z);

float get_lookAt_camX(WORLD w);

void set_lookAt_camX(WORLD w, float x);

float get_lookAt_camY(WORLD w);

void set_lookAt_camY(WORLD w, float y);

float get_lookAt_camZ(WORLD w);

void set_lookAt_camZ(WORLD w, float z);

float get_up_camX(WORLD w);

void set_up_camX(WORLD w, float x);

float get_up_camY(WORLD w);

void set_up_camY(WORLD w, float y);

float get_up_camZ(WORLD w);

void set_up_camZ(WORLD w, float z);

float get_fov(WORLD w);

float get_near(WORLD w);

float get_far(WORLD w);

float get_translate_X (TRANSFORM t);

float  get_translate_Y (TRANSFORM t);

float get_translate_Z (TRANSFORM t);

float get_rotate_angle(TRANSFORM t);

float get_rotate_X (TRANSFORM t);

float get_rotate_Y (TRANSFORM t);

float get_rotate_Z (TRANSFORM t);

float get_scale_X (TRANSFORM t);

float get_scale_Y (TRANSFORM t);

float get_scale_Z (TRANSFORM t);

std::vector<MODEL> get_models(WORLD &w);

std::vector<TRANSFORM> get_group_transforms(GROUP g);

void parse_group_element(TiXmlElement* groupElement, GROUP& group);

void parse_config_file(char* filename, WORLD& world) ;

void delete_world(WORLD &w);

#endif // XML_PARSER_H