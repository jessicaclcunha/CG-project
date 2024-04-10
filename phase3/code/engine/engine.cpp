#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <iostream>
#include <list>
#include <cmath>
#include <vector>
#include "xml_parser.hpp"
#include "../utils/figure.hpp"
#include "../utils/triangle.hpp"

#define RED 1.0f, 0.0f, 0.0f
#define GREEN 0.0f, 1.0f, 0.0f
#define BLUE 0.0f, 0.0f, 1.0f
#define WHITE 1.0f, 1.0f, 1.0f
#define PINK 1.0f, 0.75f, 0.8f
#define PURPLE 0.5f, 0.0f, 0.5f

#define CURRENT_TIME ((double)glutGet(GLUT_ELAPSED_TIME) / 1000.0) // Current time in seconds
#define START_TIME 0.0 // Start time in seconds

WORLD world;

std::vector<float> vertices; // Define vertices como vetor global
std::vector<float> normals;  // Define normals como vetor global

GLuint vbo_vertices, vbo_normals;

float camX, camY, camZ; // Camera
float LAX, LAY, LAZ;    // Look at
float upX, upY, upZ;    // Up
float fov, near, far;
float alpha = M_PI / 4, beta = M_PI / 4;
float radius;

bool axis_on = false;
bool spherical_on = false;

void changeSize(int w, int h) {
    if (h == 0)
        h = 1;

    float ratio = w * 1.0 / h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glViewport(0, 0, w, h);

    gluPerspective(get_fov(world), ratio, get_near(world), get_far(world));

    glMatrixMode(GL_MODELVIEW);
}

void draw_axis() {
    glBegin(GL_LINES);
    // X Axis
    glColor3f(PINK);
    glVertex3f(100.0f, 0.0f, 0.0f);
    glVertex3f(-100.0f, 0.0f, 0.0f);
    // Y Axis
    glColor3f(PURPLE);
    glVertex3f(0.0f, 100.0f, 0.0f);
    glVertex3f(0.0f, -100.0f, 0.0f);
    // Z Axis
    glColor3f(BLUE);
    glVertex3f(0.0f, 0.0f, 100.0f);
    glVertex3f(0.0f, 0.0f, -100.0f);
    glEnd();
}

void to_spherical() {
    camX = radius * sin(alpha) * cos(beta);
    camY = radius * sin(beta);
    camZ = radius * cos(alpha) * cos(beta);
}

void draw_figures(std::list<FIGURE> figs_list) {
    glBegin(GL_TRIANGLES);
    for (std::list<FIGURE>::iterator it = figs_list.begin(); it != figs_list.end(); ++it) {
        // Check if figure is not NULL
        if (*it != nullptr) {
            std::vector<TRIANGLE>* triangles = get_triangles(*it);
            for (TRIANGLE t : *triangles) {
                for (POINT p : *get_points(t)) {
                    glVertex3f(get_X(p), get_Y(p), get_Z(p));
                }
            }
        }
    }
    glEnd();
}

// Atualizar a função apply_transforms para renderizar grupos filhos
void apply_transforms(const GROUP& group) {
    glPushMatrix();

    // Aplicar transformações geométricas
    for (const auto& transform : get_group_transforms(group)) {
        switch (transform.type) {
            case TRANSLATE:
                glTranslatef(get_translate_X(transform), get_translate_Y(transform), get_translate_Z(transform));
                break;
            case ROTATE: {
                float angle = get_rotate_angle(transform);
                int time = get_time(transform);
                if (time > 0)
                    angle = (CURRENT_TIME - START_TIME ) * 360 / time;
                glRotatef(angle, get_rotate_X(transform), get_rotate_Y(transform), get_rotate_Z(transform));
                break;
            }
            case SCALE:
                glScalef(get_scale_X(transform), get_scale_Y(transform), get_scale_Z(transform));
                break;
        }
    }

    // Renderizar modelo usando VBOs
    glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
    glVertexPointer(3, GL_FLOAT, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_normals);
    glNormalPointer(GL_FLOAT, 0, 0);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 3);

    // Desenhar grupos filhos
    for (const auto& childGroup : group.children) {
        apply_transforms(childGroup);
    }

    glPopMatrix();
}

void renderScene(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(camX, camY, camZ, LAX, LAY, LAZ, upX, upY, upZ);

    if (axis_on)
        draw_axis();
    if (spherical_on)
        to_spherical();   
    glColor3f(WHITE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    for (const auto& group : world.groups) {
        apply_transforms(group);
    }
    glutSwapBuffers();
}

void keyboardFunc(unsigned char key, int x, int y) {
    switch (key) {
    case 'a':
        camX -= 0.1;
        break;
    case 'd':
        camX += 0.1;
        break;
    case 's':
        camZ += 0.1;
        break;
    case 'w':
        camZ -= 0.1;
        break;
    case 'q':
        alpha -= 15;
        break;
    case 'e':
        alpha += 15;
        break;
    case 'i':
        camY += 1;
        break;
    case 'k':
        camY -= 1;
        break;
    case 'j':
        LAX -= 1;
        break;
    case 'l':
        LAX += 1;
        break;
    case 'u':
        LAY -= 1;
        break;
    case 'o':
        LAY += 1;
        break;
    case '+':
        radius += 5;
        break;
    case '-':
        break;  
    case ' ':
        axis_on = !axis_on;  
        break;  
    case '0':
        spherical_on = !spherical_on;
        break;    
    }
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Phase3");

    parse_config_file(argv[1], world);
    glutInitWindowSize(get_windowWidth(world), get_windowHeight(world));
    camX = get_position_camX(world);
    camY = get_position_camY(world);
    camZ = get_position_camZ(world);
    LAX = get_lookAt_camX(world);
    LAY = get_lookAt_camY(world);
    LAZ = get_lookAt_camZ(world);
    upX = get_up_camX(world);
    upY = get_up_camY(world);
    upZ = get_up_camZ(world);
    fov = get_fov(world);
    near = get_near(world);
    far = get_far(world);
    radius = sqrt(camX * camX + camZ * camZ);
    alpha = acos(camZ / sqrt(camX * camX + camZ * camZ));
    beta = asin(camY / radius);

    std::vector<MODEL> models = get_models(world);
    int num_models = models.size();

    for (int i = 0; i < num_models; i++) {
        FIGURE figure = fileToFigure(models[i].file);
        std::vector<TRIANGLE> *triangles = get_triangles(figure);
        for (const auto& triangle : *triangles) {
            std::vector<POINT> *points = get_points(triangle);
            for (const auto& point : *points) {
                vertices.push_back(get_X(point));
                vertices.push_back(get_Y(point));
                vertices.push_back(get_Z(point));
            }
        }
    }

    // Calcular normais e adicioná-las ao vetor normals
    for (size_t i = 0; i < vertices.size(); i += 9) {
        float x1 = vertices[i];
        float y1 = vertices[i + 1];
        float z1 = vertices[i + 2];
        float x2 = vertices[i + 3];
        float y2 = vertices[i + 4];
        float z2 = vertices[i + 5];
        float x3 = vertices[i + 6];
        float y3 = vertices[i + 7];
        float z3 = vertices[i + 8];

        float nx = (y2 - y1) * (z3 - z1) - (z2 - z1) * (y3 - y1);
        float ny = (z2 - z1) * (x3 - x1) - (x2 - x1) * (z3 - z1);
        float nz = (x2 - x1) * (y3 - y1) - (y2 - y1) * (x3 - x1);

        normals.push_back(nx);
        normals.push_back(ny);
        normals.push_back(nz);
        normals.push_back(nx);
        normals.push_back(ny);
        normals.push_back(nz);
        normals.push_back(nx);
        normals.push_back(ny);
        normals.push_back(nz);
    }

    glGenBuffers(1, &vbo_vertices);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &vbo_normals);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_normals);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), &normals[0], GL_STATIC_DRAW);

    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);
    glutKeyboardFunc(keyboardFunc);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glutMainLoop();

    return 0;
}
