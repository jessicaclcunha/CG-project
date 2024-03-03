#define GL_SILENCE_DEPRECATION
#include <iostream>
#include <vector>
#include <string>
#include "../tinyxml/tinyxml.h"

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

typedef struct WORLD {
    int windowWidth;
    int windowHeight;
    Camera camera;
    Group group;
} WORLD;

void parse_config_file(const char* filename, WORLD& world) {
    TiXmlDocument doc(filename);
    if (!doc.LoadFile()) {
        std::cerr << "Error loading XML file: " << doc.ErrorDesc() << std::endl;
        return;
    }

    TiXmlElement* worldElement = doc.FirstChildElement("world");

    if (!worldElement) {
        std::cerr << "Error: Missing 'world' element in the XML file." << std::endl;
        return;
    }

    // Example: Parse window settings
    TiXmlElement* windowElement = worldElement->FirstChildElement("window");
    if (windowElement) {
        windowElement->QueryIntAttribute("width", &world.windowWidth);
        windowElement->QueryIntAttribute("height", &world.windowHeight);
    }

    // Example: Parse camera settings
    TiXmlElement* cameraElement = worldElement->FirstChildElement("camera");
    if (cameraElement) {
        // Parse camera attributes
        TiXmlElement* positionElement = cameraElement->FirstChildElement("position");
        if (positionElement) {
            positionElement->QueryFloatAttribute("x", &world.camera.position[0]);
            positionElement->QueryFloatAttribute("y", &world.camera.position[1]);
            positionElement->QueryFloatAttribute("z", &world.camera.position[2]);
        }

        TiXmlElement* lookAtElement = cameraElement->FirstChildElement("lookAt");
        if (lookAtElement) {
            lookAtElement->QueryFloatAttribute("x", &world.camera.lookAt[0]);
            lookAtElement->QueryFloatAttribute("y", &world.camera.lookAt[1]);
            lookAtElement->QueryFloatAttribute("z", &world.camera.lookAt[2]);
        }

        TiXmlElement* upElement = cameraElement->FirstChildElement("up");
        if (upElement) {
            upElement->QueryFloatAttribute("x", &world.camera.up[0]);
            upElement->QueryFloatAttribute("y", &world.camera.up[1]);
            upElement->QueryFloatAttribute("z", &world.camera.up[2]);
        }

        TiXmlElement* projectionElement = cameraElement->FirstChildElement("projection");
        if (projectionElement) {
            projectionElement->QueryFloatAttribute("fov", &world.camera.projection.fov);
            projectionElement->QueryFloatAttribute("near", &world.camera.projection.near);
            projectionElement->QueryFloatAttribute("far", &world.camera.projection.far);
        }
    }

    // Example: Parse models
    TiXmlElement* modelsElement = worldElement->FirstChildElement("group")->FirstChildElement("models");
    if (modelsElement) {
        for (TiXmlElement* modelElement = modelsElement->FirstChildElement("model"); modelElement; modelElement = modelElement->NextSiblingElement("model")) {
            Model model;
            const char* fileAttribute = modelElement->Attribute("file"); // Corrected line
            if (fileAttribute) {
                model.file = fileAttribute;
                world.group.models.push_back(model);
            } else {
                std::cerr << "Error: Model element is missing 'file' attribute." << std::endl;
            }
        }
    }
}

void delete_world(WORLD &w) {
    w.windowWidth = 0;
    w.windowHeight = 0;
    for (int i = 0; i < 3; ++i) {
        w.camera.position[i] = 0.0f;
        w.camera.lookAt[i] = 0.0f;
        w.camera.up[i] = 0.0f;
    }
    w.camera.projection.fov = 0.0f;
    w.camera.projection.near = 0.0f;
    w.camera.projection.far = 0.0f;

    /*for (Model& model : w.group.models)
        model.file.clear();*/
    w.group.models.clear();
}