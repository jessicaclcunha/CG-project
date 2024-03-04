#define GL_SILENCE_DEPRECATION
#include <stdlib.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <iostream>
#include <math.h>
#include "xml_parser.hpp"

#define RED 1.0f, 0.0f, 0.0f
#define GREEN 0.0f, 1.0f, 0.0f
#define BLUE 0.0f, 0.0f, 1.0f
#define WHITE 1.0f, 1.0f, 1.0f
#define PURPLE 0.5f, 0.0f, 0.5f
#define PINK 1.0f, 0.63f, 0.79f

WORLD world = create_world(800, 800);
float posx = 0, posz = 0, angle = 0, scalex = 1, scaley = 1, scalez = 1;

void changeSize(int w, int h) {
    // Prevent a divide by zero, when the window is too short
    // (you can't make a window with zero width).
    if (h == 0)
        h = 1;

    // compute window's aspect ratio
    float ratio = w * 1.0 / h;

    // Set the projection matrix as the current
    glMatrixMode(GL_PROJECTION);
    // Load Identity Matrix
    glLoadIdentity();

    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);

    // Set perspective
    gluPerspective(45.0f, ratio, 1.0f, 1000.0f);

    // return to the model view matrix mode
    glMatrixMode(GL_MODELVIEW);
}

void draw_axis() {
    glBegin(GL_LINES);
    // X Axis
    glColor3f(RED);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(100.0f, 0.0f, 0.0f);
    // Y Axis
    glColor3f(GREEN);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 100.0f, 0.0f);
    // Z Axis
    glColor3f(BLUE);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 100.0f);
    glEnd();
}

void renderScene(void) {
    // clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // set the camera
    glLoadIdentity();
    gluLookAt(5.0, 5.0, 5.0,
              0.0, 0.0, 0.0,
              0.0f, 0.0f, 0.0f);

    // put drawing instructions here
    draw_axis();
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_TRIANGLES);
	//função de desenhar as funções

    // End of frame
    glutSwapBuffers();
}

// write function to process keyboard events

void keyboardFunc(unsigned char key, int x, int y) {
    switch (key) {
        case 'a':
            posx -= 0.1;
            break;
        case 'd':
            posx += 0.1;
            break;
        case 's':
            posz += 0.1;
            break;
        case 'w':
            posz -= 0.1;
            break;
        case 'q':
            angle -= 15;
            break;
        case 'e':
            angle += 15;
            break;
        case 'i':
            scalez += 0.1;
            break;
        case 'k':
            scalez -= 0.1;
            break;
        case 'j':
            scalex -= 0.1;
            break;
        case 'l':
            scalex += 0.1;
            break;
        case 'u':
            scaley -= 0.1;
            break;
        case 'o':
            scaley += 0.1;
            break;
        case '+':
            scalex += 0.1;
            scaley += 0.1;
            scalez += 0.1;
            break;
        case '-':
            scalex -= 0.1;
            scaley -= 0.1;
            scalez -= 0.1;
            break;
    }
    glutPostRedisplay();
}

int main(int argc, char **argv) {
    // Check if the correct number of command-line arguments is provided
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <config_file.xml>" << std::endl;
        return 1;
    }

    // init GLUT and the window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Phase1");

    parse_config_file(argv[1], world);

    // Required callback registry
    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);

    // put here the registration of the keyboard callbacks
    glutKeyboardFunc(keyboardFunc);

    // OpenGL settings
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    // enter GLUT's main cycle
    glutMainLoop();

    // Clean up allocated memory
    delete_world(world);

    return 0;
}
