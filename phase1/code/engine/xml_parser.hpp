// xml_parser.hpp
#ifndef XML_PARSER_H
#define XML_PARSER_H


#include <iostream>
#include <vector>
#include <string>
#include "../tinyxml/tinyxml.h"

struct Camera ;

struct Model;

struct Group ;

typedef struct World *WORLD;

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

WORLD create_world(int windowWidth, int windowHeight);

void parse_config_file(const char* filename, WORLD world);

void delete_world(WORLD &w);

#endif // XML_PARSER_H