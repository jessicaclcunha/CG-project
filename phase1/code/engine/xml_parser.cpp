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

typedef struct World {
    int windowWidth;
    int windowHeight;
    Camera camera;
    Group group;
} *WORLD;

float get_position_camX(WORLD w) {
    return w->camera.position[0];
}

void set_position_camX(WORLD w, float x) {
    w->camera.position[0] = x;
}

float get_position_camY(WORLD w) {
    return w->camera.position[1];
}

void set_position_camY(WORLD w, float y) {
    w->camera.position[1] = y;
}

float get_position_camZ(WORLD w) {
    return w->camera.position[2];
}

void set_position_camZ(WORLD w, float z) {
    w->camera.position[2] = z;
}

float get_lookAt_camX(WORLD w) {
    return w->camera.lookAt[0];
}

void set_lookAt_camX(WORLD w, float x) {
    w->camera.lookAt[0] = x;
}

float get_lookAt_camY(WORLD w) {
    return w->camera.lookAt[1];
}

void set_lookAt_camY(WORLD w, float y) {
    w->camera.lookAt[1] = y;
}

float get_lookAt_camZ(WORLD w) {
    return w->camera.lookAt[2];
}

void set_lookAt_camZ(WORLD w, float z) {
    w->camera.lookAt[2] = z;
}

float get_up_camX(WORLD w) {
    return w->camera.up[0];
}

void set_up_camX(WORLD w, float x) {
    w->camera.up[0] = x;
}

float get_up_camY(WORLD w) {
    return w->camera.up[1];
}

void set_up_camY(WORLD w, float y) {
    w->camera.up[1] = y;
}

float get_up_camZ(WORLD w) {
    return w->camera.up[2];
}

void set_up_camZ(WORLD w, float z) {
    w->camera.up[2] = z;
}

WORLD create_world(int windowWidth, int windowHeight) {
    WORLD w =  (WORLD)malloc(sizeof(struct World));
    w->windowWidth = windowWidth;
    w->windowHeight = windowHeight;
    return w;
}

// Função auxiliar para analisar atributos de vetor (x, y, z)
void parse_vector_attribute(TiXmlElement* parentElement, const char* attributeName, float* vector) {
    TiXmlElement* vectorElement = parentElement->FirstChildElement(attributeName);
    if (vectorElement) {
        vectorElement->QueryFloatAttribute("x", &vector[0]);
        vectorElement->QueryFloatAttribute("y", &vector[1]);
        vectorElement->QueryFloatAttribute("z", &vector[2]);
    }
}

void parse_config_file(const char* filename, WORLD world) {
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

    // Parse window settings
    TiXmlElement* windowElement = worldElement->FirstChildElement("window");
    if (windowElement) {
        windowElement->QueryIntAttribute("width", &world->windowWidth);
        windowElement->QueryIntAttribute("height", &world->windowHeight);
    }

    // Parse camera settings
    TiXmlElement* cameraElement = worldElement->FirstChildElement("camera");
    if (cameraElement) {
        // Parse camera attributes
        parse_vector_attribute(cameraElement, "position", world->camera.position);
        parse_vector_attribute(cameraElement, "lookAt", world->camera.lookAt);
        parse_vector_attribute(cameraElement, "up", world->camera.up);

        // Parse projection element
        TiXmlElement* projectionElement = cameraElement->FirstChildElement("projection");
        if (projectionElement) {
            projectionElement->QueryFloatAttribute("fov", &world->camera.projection.fov);
            projectionElement->QueryFloatAttribute("near", &world->camera.projection.near);
            projectionElement->QueryFloatAttribute("far", &world->camera.projection.far);
        }
    }

    // Parse models
    TiXmlElement* modelsElement = worldElement->FirstChildElement("group")->FirstChildElement("models");
    if (modelsElement) {
        for (TiXmlElement* modelElement = modelsElement->FirstChildElement("model"); modelElement; modelElement = modelElement->NextSiblingElement("model")) {
            Model model;
            const char* fileAttribute = modelElement->Attribute("file");
            if (fileAttribute) {
                model.file = fileAttribute;
                world->group.models.push_back(model);
            } else {
                std::cerr << "Error: Model element is missing 'file' attribute." << std::endl;
            }
        }
    }
}

void delete_world(WORLD &w) {
    w->windowWidth = 0;
    w->windowHeight = 0;
    for (int i = 0; i < 3; ++i) {
        w->camera.position[i] = 0.0f;
        w->camera.lookAt[i] = 0.0f;
        w->camera.up[i] = 0.0f;
    }
    w->camera.projection.fov = 0.0f;
    w->camera.projection.near = 0.0f;
    w->camera.projection.far = 0.0f;

    /*for (Model& model : w.group.models)
        model.file.clear();*/
    w->group.models.clear();
}