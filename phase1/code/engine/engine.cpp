#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <iostream>
#include <list>
#include "xml_parser.hpp"
#include "../utils/figure.hpp"
#include "../utils/triangle.hpp"

#define RED 1.0f, 0.0f, 0.0f
#define GREEN 0.0f, 1.0f, 0.0f
#define BLUE 0.0f, 0.0f, 1.0f
#define WHITE 1.0f, 1.0f, 1.0f
#define PINK 1.0f, 0.75f, 0.8f
#define PURPLE 128, 0, 128


WORLD world = create_world();
std::list<FIGURE> figures_list;

float camX, camY, camZ; //camera
float LAX, LAY, LAZ; //look at
float upX, upY, upZ; //up
float fov, near, far;
float alpha = M_PI / 4, beta = M_PI / 4;
float radius;

void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if(h == 0)
		h = 1;

	// compute window's aspect ratio 
	float ratio = w * 1.0 / h;

	// Set the projection matrix as current
	glMatrixMode(GL_PROJECTION);
	// Load Identity Matrix
	glLoadIdentity();
	
	// Set the viewport to be the entire window
    glViewport(0, 0, w, h);

	// Set perspective
	gluPerspective(45.0f ,ratio, 1.0f ,1000.0f);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}


void draw_axis() {
	glBegin(GL_LINES);
	//X Axis
	glColor3f(RED);
	glVertex3f( 0.0f, 0.0f, 0.0f);
    glVertex3f( 100.0f, 0.0f, 0.0f);
    //Y Axis
    glColor3f(GREEN);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 100.0f, 0.0f);
    //Z Axis
    glColor3f(BLUE);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 100.0f);
    glEnd();
}


/*void draw_figures(std::list<FIGURE> figs_list) {
	for (std::list<FIGURE>::iterator it = figs_list.begin(); it != figs_list.end(); ++it){
        // Check if figure is not NULL
        if (*it != nullptr) {
			std::vector<TRIANGLE>* triangles = get_triangles(*it);
            std::vector<POINT>* vertices = get_vertices(*it);

            // Draw the figure using OpenGL commands
            glBegin(GL_TRIANGLES);

            for (const POINT& vertex : *vertices) {
                glVertex3f(get_X(vertex), get_Y(vertex), get_Z(vertex));
            }
        }
    }
}*/


void renderScene(void) {

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(camX, camY, camZ,
			  LAX, LAY, LAZ,
			  upX, upY, upZ);



// put drawing instructions here
	draw_axis();
	glColor3f(PURPLE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_TRIANGLES);
	//draw_figures(figures_list);
	glEnd();
	// End of frame
	glutSwapBuffers();
}



// write function to process keyboard events

void keyboardFunc(unsigned char key, int x, int y) {
    switch(key) {
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

		// nao estao a funcionar
        case 'q':
            alpha -= 15;
            break;
        case 'e':
            alpha += 15;
            break;

			
        case 'i':
            camY += 0.1;
            break;
        case 'k':
            camY -= 0.1;
            break;
        case 'j':
            LAX -= 0.1;
            break;
        case 'l':
            LAX += 0.1;
            break;
        case 'u':
            LAY -= 0.1;
            break;
        case 'o':
            LAY += 0.1;
            break;	
    }
    glutPostRedisplay();
}


int main(int argc, char **argv) {

// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(800,800);
	glutCreateWindow("Phase1");

    parse_config_file(argv[1], world);
	camX = get_position_camX (world);
	camY = get_position_camY (world);
	camZ = get_position_camZ (world);
	LAX = get_lookAt_camX(world);
	LAY = get_lookAt_camY(world);
	LAZ = get_lookAt_camZ(world);
	upX = get_up_camX(world);
	upY = get_up_camY(world);
	upZ = get_up_camZ(world);
	fov = get_fov(world);
	near = get_near(world);
	far = get_far(world);
	alpha = acos(camZ / sqrt(camX * camX + camZ * camZ));
	beta = asin(camY / radius);

	std::vector<Model> models = get_models(world);
	int num_models = models.size();
	for (Model m : models)
		figures_list.push_back(file2figure(m));

	
// Required callback registry 
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);

	
// put here the registration of the keyboard callbacks

	glutKeyboardFunc(keyboardFunc);

//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	
// enter GLUT's main cycle
	glutMainLoop();

	//delete_world(world);
	
	return 1;
}