#include <iostream>
#include <vector>
#include <string>
#include "tinyxml2.h"

struct Camera {
    float position[3];
    float lookAt[3];
    float up[3];
    struct Projection {
        float fov;
        float near;
        float far;
    } projection;
};

struct Model {
    std::string file;
};

struct Group {
    std::vector<Model> models;
};

struct World {
    int windowWidth;
    int windowHeight;
    Camera camera;
    Group group;
};

void parse_config_file(const char* filename, World& world) {
    tinyxml2::XMLDocument doc;
    doc.LoadFile(filename);

    if (doc.Error()) {
        std::cerr << "Error loading XML file: " << doc.ErrorStr() << std::endl;
        return;
    }

    // Parse XML elements and fill the World structure
    // ...

    // Example: Parse window settings
    tinyxml2::XMLElement* windowElement = doc.FirstChildElement("world")->FirstChildElement("window");
    if (windowElement) {
        windowElement->QueryIntAttribute("width", &world.windowWidth);
        windowElement->QueryIntAttribute("height", &world.windowHeight);
    }

    // Example: Parse camera settings
    tinyxml2::XMLElement* cameraElement = doc.FirstChildElement("world")->FirstChildElement("camera");
    if (cameraElement) {
        // Parse camera attributes
    }

    // Example: Parse models
    tinyxml2::XMLElement* modelsElement = doc.FirstChildElement("world")->FirstChildElement("group")->FirstChildElement("models");
    for (tinyxml2::XMLElement* modelElement = modelsElement->FirstChildElement("model"); modelElement; modelElement = modelElement->NextSiblingElement("model")) {
        Model model;
        modelElement->QueryStringAttribute("file", &model.file);
        world.group.models.push_back(model);
    }
}

int main() {
    World world;
    parse_config_file("config.xml", world);

    // Implement rendering using the parsed data
    // ...

    return 0;
}
