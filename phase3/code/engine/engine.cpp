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

#define RED 1.0f, 0.0f, 0.0f
#define GREEN 0.0f, 1.0f, 0.0f
#define BLUE 0.0f, 0.0f, 1.0f
#define WHITE 1.0f, 1.0f, 1.0f
#define PINK 1.0f, 0.75f, 0.8f
#define PURPLE 0.5f, 0.0f, 0.5f

#define CURRENT_TIME ((double)glutGet(GLUT_ELAPSED_TIME) / 1000.0) // Current time in seconds
#define START_TIME 0.0 // Start time in seconds

WORLD world;

GLuint *buffers; // Array of buffer IDs for each figure
unsigned int figs_count = 0; // Number of figures
std::vector<unsigned int> buffers_sizes; // Size (in vertices) of each figure

float camX = 0, camY = 0, camZ = 0; // Camera
float LAX = 0, LAY = 0, LAZ = 0;    // Look at
float upX = 0, upY = 1, upZ = 0;    // Up (defaulting to y-axis)
float fov = 60, near = 0.1, far = 1000; // Field of view, near and far clipping planes
float alpha = M_PI / 4, beta = M_PI / 4;
float radius = 10; // Default radius

bool axis_on = false;
bool spherical_on = false;

void changeSize(int w, int h) {
    if (h == 0)
        h = 1;

    float ratio = w * 1.0 / h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glViewport(0, 0, w, h);

    gluPerspective(fov, ratio, near, far);

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

void init_vbo(std::vector<GROUP>& groups, int *ind) {
    buffers = new GLuint[figs_count];
    glGenBuffers(figs_count, buffers);
    for (size_t i = 0; i < groups.size(); ++i) {
        for (const auto& model : groups[i].models) {
            FIGURE figure = fileToFigure(model.file);
            std::vector<float> fig_vectors = figure_to_vectores(figure);
            glBindBuffer(GL_ARRAY_BUFFER, buffers[(*ind)++]);
            glBufferData(GL_ARRAY_BUFFER, fig_vectors.size() * sizeof(float), fig_vectors.data(), GL_STATIC_DRAW);
            buffers_sizes.push_back(fig_vectors.size()/3);
        }
        init_vbo(groups[i].children, ind);
    }
}

void apply_transforms(const GROUP& group, int *index) {
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
                    angle = fmod((CURRENT_TIME - START_TIME) * 360 / time, 360);
                glRotatef(angle, get_rotate_X(transform), get_rotate_Y(transform), get_rotate_Z(transform));
                break;
            }
            case SCALE:
                glScalef(get_scale_X(transform), get_scale_Y(transform), get_scale_Z(transform));
                break;
        }
    }

    for (const auto& model : group.models) {
        glBindBuffer(GL_ARRAY_BUFFER, buffers[*index]);
        glVertexPointer(3, GL_FLOAT, 0, 0);
        glDrawArrays(GL_TRIANGLES, 0, buffers_sizes[*index]);
    }

    for (const auto& childGroup : group.children) {
        apply_transforms(childGroup, index);
    }

    glPopMatrix();
}

void renderScene(void) {
    // Clear any previous errors
    while (glGetError() != GL_NO_ERROR);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(camX, camY, camZ, LAX, LAY, LAZ, upX, upY, upZ);

    if (axis_on)
        draw_axis();
    if (spherical_on)
        to_spherical();

    glColor3f(WHITE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    int index = 0;
    for (const auto& group : world.groups) {
        apply_transforms(group, &index);

        // Check for OpenGL errors after each group rendering
        GLenum error = glGetError();
        if (error != GL_NO_ERROR) {
            std::cerr << "Error rendering group: " << gluErrorString(error) << std::endl;
        }
    }

    // Check for OpenGL errors at the end of rendering
    GLenum finalError = glGetError();
    if (finalError != GL_NO_ERROR) {
        std::cerr << "Final rendering error: " << gluErrorString(finalError) << std::endl;
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
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <config_file.xml>" << std::endl;
        return 1;
    }

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

    std::vector<GROUP> groups = get_groups(world);
    
    for (const auto& group : groups) {
        figs_count += get_figs_count(group);
    }

    int index = 0;
    init_vbo(groups, &index);

    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);
    glutKeyboardFunc(keyboardFunc);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glutMainLoop();

    delete[] buffers;

    return 0;
}