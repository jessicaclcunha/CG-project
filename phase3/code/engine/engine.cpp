#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
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
int init_time;
int elapsed_time = 0;

WORLD world;

GLuint *buffers = NULL; // Array of buffer IDs for each figure
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

void init_vbo(const std::vector<MODEL>& models, int *index) {
    buffers = new GLuint[models.size()];
    glGenBuffers(models.size(), buffers);
    for (size_t i = 0; i < models.size(); ++i) {
        FIGURE figure = fileToFigure(models[i].file);
        std::vector<float> fig_vectors = figure_to_vectors(figure);

        size_t total_size = fig_vectors.size();

        // Debug messages
        std::cout << "Model " << i << " file: " << models[i].file << std::endl;

        // Gera e vincula o buffer de vértices
        glBindBuffer(GL_ARRAY_BUFFER, buffers[*index]);
        glBufferData(GL_ARRAY_BUFFER, total_size * sizeof(float), fig_vectors.data(), GL_STATIC_DRAW);
        buffers_sizes.push_back(total_size/3);
        (*index)++;
    }
}



void apply_transforms(const GROUP& group, unsigned int *index) {
    glPushMatrix();

    for (const auto& transform : get_group_transforms(group)) 
    {
        switch (transform.type) 
        {
            case TRANSLATE:
            {    
                int time = get_time(transform);
                if(time > 0) {
                    float t = CURRENT_TIME / time; // Calculate the interpolation parameter
                    std::vector<POINT> points = get_transform_points(transform);

                    std::cout << "Before catmullRom_curve function call" << std::endl;
                    POINT interpolatedPoint = catmullRom_curve(points, t);
                    std::cout << "After catmullRom_curve function call" << std::endl;
                    
                    glTranslatef(get_X(interpolatedPoint), get_Y(interpolatedPoint), get_Z(interpolatedPoint));  

                    if(get_align(transform)) 
                    {
                        POINT directionVector = new_point(0.0f, 0.0f, 0.0f);
                        if (t < 1.0f) {
                            printf("before");
                            POINT nextPoint = catmullRom_curve(get_transform_points(transform), t + 0.01f); // Get next point slightly ahead
                            printf("after");
                            set_X(directionVector, get_X(nextPoint) - get_X(interpolatedPoint));
                            set_Y(directionVector, get_Y(nextPoint) - get_Y(interpolatedPoint));
                            set_Z(directionVector, get_Z(nextPoint) - get_Z(interpolatedPoint));
                        } else {
                            printf("else before");
                            // If at the end of the curve, use the previous point
                            POINT prevPoint = catmullRom_curve(transform.points, t - 0.01f); // Get previous point slightly behind
                            printf("else after");
                            set_X(directionVector, get_X(interpolatedPoint) - get_X(prevPoint));
                            set_Y(directionVector, get_Y(interpolatedPoint) - get_Y(prevPoint));
                            set_Z(directionVector, get_Z(interpolatedPoint) - get_Z(prevPoint));
                        }

                        // Normalize the direction vector
                        normalize_vector(directionVector);

                        // Calculate the new Y axis (transform Y axis)
                        POINT transformYAxis = new_point(0.0f, 1.0f, 0.0f);

                        POINT z;
                        cross_product(directionVector, transformYAxis, z); // Calculate the X axis (z)
                        normalize_vector(z);

                        POINT y;
                        cross_product(z, directionVector, y); // Calculate the Y axis (y)
                        normalize_vector(y);
                        
                        // Build the rotation matrix
                        float rot[16];
                        buildRotMatrix(directionVector, y, z, rot);

                        // Multiply the current modelview matrix by the rotation matrix
                        glMultMatrixf(rot);
                    }    
                } else {
                    glTranslatef(get_translate_X(transform), get_translate_Y(transform), get_translate_Z(transform));
                }
                break;
            }
            case ROTATE: 
            {
                float angle = get_rotate_angle(transform);
                int time = get_time(transform);
                if (time > 0)
                    angle = (CURRENT_TIME - init_time) * 360 / time;
                glRotatef(angle, get_rotate_X(transform), get_rotate_Y(transform), get_rotate_Z(transform));
                break;
            }
            case SCALE:
            {    
                glScalef(get_scale_X(transform), get_scale_Y(transform), get_scale_Z(transform));
                break;
            }
        }
    }

    glEnableClientState(GL_VERTEX_ARRAY);

    for (const auto& model : group.models) 
    {
        if (*index < buffers_sizes.size()) 
        {
            glBindBuffer(GL_ARRAY_BUFFER, buffers[*index]);  // Use buffer for this figure
            glVertexPointer(3, GL_FLOAT, 0, 0);
            glDrawArrays(GL_TRIANGLES, 0, buffers_sizes[*index]);
            (*index)++;
            printf("Index: %d\n", *index);
        } 
        else {
            //printf("Index: %d\n", *index);
            //printf("Buffers_sizes size: %lu\n", buffers_sizes.size());
            //std::cerr << "Index out of range for buffers_sizes!" << std::endl;
        }
        
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
        apply_transforms(group, &index);
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
	radius = sqrt(camX * camX + camY * camY + camZ * camZ);
    alpha = acos(camZ / radius);
    beta = asin(camY / radius);

    std::vector<MODEL> models = get_models(world);

    int index = 0;
    init_vbo(models, &index);

    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);
    glutKeyboardFunc(keyboardFunc);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    elapsed_time = glutGet(GLUT_ELAPSED_TIME);
    init_time = CURRENT_TIME;


    glutMainLoop();

    delete[] buffers;  // Deallocate memory for buffers array
    return 0;
}
