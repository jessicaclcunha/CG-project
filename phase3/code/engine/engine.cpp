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

GLuint *buffers = NULL; // Array of buffer IDs for each figure
GLuint *buffer_indices = NULL; // Array of index buffer IDs for each figure
unsigned int figs_count = 0; // Number of figures
std::vector<unsigned int> buffers_sizes; // Size (in vertices) of each figure

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

void init_vbo(const std::vector<MODEL>& models) {
    // Allocate memory for buffers array
    buffers = new GLuint[models.size()];
    buffer_indices = new GLuint[models.size()];

    for (size_t i = 0; i < models.size(); ++i) {
        FIGURE figure = fileToFigure(models[i].file);
        std::vector<float> fig_vectores = figure_to_vectores(figure);

        size_t total_size = fig_vectores.size();

        // Generate and bind vertex buffer
        glGenBuffers(1, &buffers[i]);
        glBindBuffer(GL_ARRAY_BUFFER, buffers[i]);
        glBufferData(GL_ARRAY_BUFFER, total_size * sizeof(float), fig_vectores.data(), GL_STATIC_DRAW);

        // Generate and bind index buffer
        glGenBuffers(1, &buffer_indices[i]);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_indices[i]);  // Assuming each figure uses an index buffer
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, total_size / 3 * sizeof(unsigned int), nullptr, GL_STATIC_DRAW);

        // Populate index buffer with sequential indices (modify if needed)
        std::vector<unsigned int> indices(total_size / 3);
        for (size_t j = 0; j < total_size / 3; ++j)
            indices[j] = j;

        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, total_size / 3 * sizeof(unsigned int), indices.data());

        buffers_sizes.push_back(total_size / 3);
    }
}


void apply_transforms(const GROUP& group, unsigned int &index) {
    glPushMatrix();

    for (const auto& transform : get_group_transforms(group)) {
        switch (transform.type) {
            case TRANSLATE:
                glTranslatef(get_translate_X(transform), get_translate_Y(transform), get_translate_Z(transform));
                break;
            case ROTATE: {
                float angle = get_rotate_angle(transform);
                int time = get_time(transform);
                if (time > 0)
                    angle = (CURRENT_TIME - START_TIME) * 360 / time;
                glRotatef(angle, get_rotate_X(transform), get_rotate_Y(transform), get_rotate_Z(transform));
                break;
            }
            case SCALE:
                glScalef(get_scale_X(transform), get_scale_Y(transform), get_scale_Z(transform));
                break;
        }
    }

    glEnableClientState(GL_VERTEX_ARRAY);

    unsigned int localIndex = 0;
    for (const auto& model : group.models) {
        size_t buffer_index = index + localIndex;
        glBindBuffer(GL_ARRAY_BUFFER, buffers[buffer_index]);  // Use buffer for this figure
        glVertexPointer(3, GL_FLOAT, 0, 0);
        glDrawArrays(GL_TRIANGLES, 0, buffers_sizes[buffer_index]);

        localIndex++;
    }
    glDisableClientState(GL_VERTEX_ARRAY);

    for (const auto& childGroup : group.children) {
        apply_transforms(childGroup, index);
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

    unsigned int index = 0;
    for (const auto& group : world.groups) {
        apply_transforms(group, index);
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

    init_vbo(models);

    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);
    glutKeyboardFunc(keyboardFunc);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glutMainLoop();

    delete[] buffers;  // Deallocate memory for buffers array
    delete[] buffer_indices;  // Deallocate memory for index buffers array

    return 0;
}


