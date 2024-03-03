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

typedef struct World WORLD;

void parse_config_file(const char* filename, World& world);

void delete_world(WORLD &w);

#endif // XML_PARSER_H