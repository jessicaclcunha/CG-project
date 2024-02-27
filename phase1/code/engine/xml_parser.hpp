// xml_parser.hpp
#ifndef XML_PARSER_H
#define XML_PARSER_H


#include <iostream>
#include <vector>
#include <string>
#include "../tinyxml/tinyxml.h"

struct Camera CAMERA;

struct Model;

struct Group GROUP;

typedef struct World WORLD;

void parse_config_file(const char* filename, World& world);

#endif // XML_PARSER_H