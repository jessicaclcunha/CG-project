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
#include "catmull.hpp"

#define RED 1.0f, 0.0f, 0.0f
#define GREEN 0.0f, 1.0f, 0.0f
#define BLUE 0.0f, 0.0f, 1.0f
#define WHITE 1.0f, 1.0f, 1.0f
#define PINK 1.0f, 0.75f, 0.8f
#define PURPLE 0.5f, 0.0f, 0.5f

int init_time = 0;
int elapsed_time = 0;

WORLD world;

GLuint *buffers = NULL; // Array of buffer IDs for each figure
GLuint *normalBuffers = NULL;
GLuint *texBuffers = NULL;
std::vector<unsigned int> buffers_sizes; // Size (in vertices) of each figure
std::vector<unsigned int> normalsbuf_size;

float camX, camY, camZ; // Camera
float LAX, LAY, LAZ;    // Look at
float upX, upY, upZ;    // Up
float fov, near, far;
float alpha = M_PI / 4, beta = M_PI / 4;
float radius;
float frames = 0;

bool axis_on = false;
bool spherical_on = false;
bool trajectory_on = false;
bool normais_on = false;


void framerate() {
    frames++;
    char title[50];
    int time = glutGet(GLUT_ELAPSED_TIME);
    if (time - elapsed_time > 1000) {
        sprintf(title, "PHASE3 - Engine | FPS: %4.2f", frames * 1000.0 / (time - elapsed_time));
        glutSetWindowTitle(title);
        elapsed_time = time;
        frames = 0;
    }

}

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
    glVertex3f(1000.0f, 0.0f, 0.0f);
    glVertex3f(-1000.0f, 0.0f, 0.0f);
    // Y Axis
    glColor3f(PURPLE);
    glVertex3f(0.0f, 1000.0f, 0.0f);
    glVertex3f(0.0f, -1000.0f, 0.0f);
    // Z Axis
    glColor3f(BLUE);
    glVertex3f(0.0f, 0.0f, 1000.0f);
    glVertex3f(0.0f, 0.0f, -1000.0f);
    glEnd();
}

void to_spherical() {
    camX = radius * sin(alpha) * cos(beta);
    camY = radius * sin(beta);
    camZ = radius * cos(alpha) * cos(beta);
}

int get_nlight(int nLight) {
    int light;
    switch (nLight) {
        case 0: light = GL_LIGHT0; break;
        case 1: light = GL_LIGHT1; break;
        case 2: light = GL_LIGHT2; break;
        case 3: light = GL_LIGHT3; break;
        case 4: light = GL_LIGHT4; break;
        case 5: light = GL_LIGHT5; break;
        case 6: light = GL_LIGHT6; break;
        case 7: light = GL_LIGHT7; break;
        default: exit(1);
    }
    return light;
}

void init_vbo(const std::vector<MODEL>& models, int *index, std::vector<LIGHT> &lights) {
    buffers = new GLuint[models.size()];
    glGenBuffers(models.size(), buffers);

    normalBuffers = new GLuint[models.size()];
    glGenBuffers(models.size(), normalBuffers);

    texBuffers = new GLuint[models.size()];
    glGenBuffers(models.size(), texBuffers);

    for (size_t i = 0; i < models.size(); ++i) {
        FIGURE figure = fileToFigure(models[i].file);
        std::vector<float> fig_vectors = figure_to_vectors(figure);
        std::vector<float> fig_normals = figure_to_normals(figure);
        std::vector<float> fig_textures = figure_to_textures(figure);
        const std::string texture = models[i].texture_file;

        size_t total_size = fig_vectors.size();

        // Generate and bind the vertex buffer
        glBindBuffer(GL_ARRAY_BUFFER, buffers[*index]);
        glBufferData(GL_ARRAY_BUFFER, total_size * sizeof(float), fig_vectors.data(), GL_STATIC_DRAW);
        buffers_sizes.push_back(total_size / 3);

        // Generate and bind the normal buffer
        if (fig_normals.size() > 0) {
            normalBuffers = new GLuint[models.size()];
            glGenBuffers(models.size(), normalBuffers);
            glBindBuffer(GL_ARRAY_BUFFER, normalBuffers[*index]);
            glBufferData(GL_ARRAY_BUFFER, fig_normals.size() * sizeof(float), fig_normals.data(), GL_STATIC_DRAW);
            normalsbuf_size.push_back(fig_normals.size() / 3);
        }

        // Generate and bind the texture buffer
        if (!texture.empty()) {
            texBuffers = new GLuint[models.size()];
            glGenBuffers(models.size(), texBuffers);
            glBindBuffer(GL_ARRAY_BUFFER, texBuffers[*index]);
            glBufferData(GL_ARRAY_BUFFER, fig_textures.size() * sizeof(float), fig_textures.data(), GL_STATIC_DRAW);
        }
        (*index)++;
    }


    if(lights.size() > 0)
    {
        glEnable(GL_LIGHTING); 
		glEnable(GL_RESCALE_NORMAL);
        GLfloat white[4] = {1.0,1.0,1.0,1.0};
		for(int i = 0; i < lights.size(); i++){
			glEnable(get_nlight(i));
            glLightfv(get_nlight(i), GL_DIFFUSE, white);
            glLightfv(get_nlight(i), GL_SPECULAR, white);
		}
		
		float amb[4] = { 1.0f, 1.0f, 1.0f, 0.4f };
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb);
    	
	}
    
}

void renderCatmullRomCurve(std::vector<POINT> points, int numSamples) {
    float pos[3];
    glBegin(GL_LINE_LOOP);
    float gt = 0;
    for (int i = 0; i < numSamples; i++) {
        gt += 1.0 / numSamples;
        getGlobalCatmullRomPoint(gt, points, pos, NULL);
        glVertex3f(pos[0], pos[1], pos[2]);
    }
    glEnd();
}

void apply_transforms(const GROUP& group, unsigned int *index) {
    for (const auto& transform : get_group_transforms(group)) 
    {
        switch (transform.type) 
        {
            case SCALE:
            {    
                glScalef(get_scale_X(transform), get_scale_Y(transform), get_scale_Z(transform));
                break;
            }
            case ROTATE: 
            {
                float angle = get_rotate_angle(transform);
                int time = get_time(transform);
                if (time > 0)
                    angle = (((glutGet(GLUT_ELAPSED_TIME) / 1000.0f) - init_time) * 360) / time;
                while (angle > 360)
                    angle -= 360;
                glRotatef(angle, get_rotate_X(transform), get_rotate_Y(transform), get_rotate_Z(transform));
                break;
            }
            case TRANSLATE:
            {    
                int time = get_time(transform);
                if(time > 0) {
                    float t = (glutGet(GLUT_ELAPSED_TIME) / 1000.0) / time; // Calculate the interpolation parameter
                    std::vector<POINT> points = get_transform_points(transform);

                    float pos[3], deriv[3]; //x

                    getGlobalCatmullRomPoint(t, points, pos, deriv);

                    if(trajectory_on)
                        renderCatmullRomCurve(points, 100); // Render the curve

                    glTranslatef(pos[0], pos[1], pos[2]);

                    POINT derivx = new_point(deriv[0], deriv[1], deriv[2]); 

                    if(get_align(transform)) 
                    {   
                        normalize(derivx); // x
                        
                        POINT z = NULL;
                        cross(derivx, get_y_aux(transform), z); // z
                        normalize(z); // z

                        POINT y = NULL;
                        cross(z, derivx, y); // y
                        set_y_aux(transform, y); // y
                        normalize(y); // y
                        
                        // Build the rotation matrix
                        float m[16];

                        buildRotMatrix(derivx, y, z, m); // x, y, z

                        // Multiply the current modelview matrix by the rotation matrix
                        glMultMatrixf(m);
                    }  
                } 
                else 
                {
                    glTranslatef(get_translate_X(transform), get_translate_Y(transform), get_translate_Z(transform));
                }
                break;
            }
        }
    }
}


void apply_normals(MODEL model) {
    if (get_lights(world).size() > 0)
        glDisable(GL_LIGHTING);
    glColor3f(RED);
    FIGURE figure = fileToFigure(model.file);
    std::vector<float> normals = figure_to_normals(figure);
    std::vector<float> points = figure_to_vectors(figure);
    glBegin(GL_LINES);
    for (size_t i = 0; i < normals.size(); i+=3) {
        glVertex3f(points[i],points[i+1],points[i+2]);
        glVertex3f(points[i] + normals[i],points[i+1] + normals[i+1],points[i+2] + normals[i+2]);
    }
    glEnd();
    if (get_lights(world).size() > 0)
        glEnable(GL_LIGHTING);

}


void draw_figures(const GROUP &g, unsigned int *index) {
    
    printf("Drawing :)\n");
    glPushMatrix();

    apply_transforms(g, index);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    // Ensure lighting is enabled
    glEnable(GL_LIGHTING);

    for (const auto& model : g.models) {
        if(normais_on)
            apply_normals(model);
        if (*index < buffers_sizes.size()) {
            std::vector<COLOR> colors = get_colors(model);
            for (const COLOR &c : colors) {
                GLfloat aux[4] = {1.0f, 1.0f, 1.0f, 1.0f}; // Initialize with 4 elements for RGBA
                switch (c.type) {
                    case C_DIFFUSE:
                        aux[0] = c.diffuse.r;
                        aux[1] = c.diffuse.g;
                        aux[2] = c.diffuse.b;
                        glMaterialfv(GL_FRONT, GL_DIFFUSE, aux);
                        break;
                    case C_AMBIENT:
                        aux[0] = c.ambient.r;
                        aux[1] = c.ambient.g;
                        aux[2] = c.ambient.b;
                        glMaterialfv(GL_FRONT, GL_AMBIENT, aux);
                        break;
                    case C_SPECULAR:
                        aux[0] = c.specular.r;
                        aux[1] = c.specular.g;
                        aux[2] = c.specular.b;
                        glMaterialfv(GL_FRONT, GL_SPECULAR, aux);
                        break;
                    case C_EMISSIVE:
                        aux[0] = c.emissive.r;
                        aux[1] = c.emissive.g;
                        aux[2] = c.emissive.b;
                        glMaterialfv(GL_FRONT, GL_EMISSION, aux);
                        break;
                    case C_SHININESS:
                        glMaterialf(GL_FRONT, GL_SHININESS, c.shininess.value);
                        break;
                    default:
                        break;
                }
            }
            if (!normalsbuf_size.empty()) {
                glBindBuffer(GL_ARRAY_BUFFER, normalBuffers[*index]);
                glNormalPointer(GL_FLOAT, 0, 0);
            } else {
                printf("Normais vazias??\n");
            }
            
            glBindBuffer(GL_ARRAY_BUFFER, buffers[*index]);  // Use buffer for this figure
            glVertexPointer(3, GL_FLOAT, 0, 0);

            
            glDrawArrays(GL_TRIANGLES, 0, buffers_sizes[*index]);
            (*index)++;
        } else {
            std::cerr << "Index out of range for buffers_sizes!" << std::endl;
        }
    }

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);

    // Disable lighting after rendering
    glDisable(GL_LIGHTING);

    std::vector<GROUP> children = get_group_children(g);

    // Debug output to check children count
    std::cout << "Group has " << children.size() << " children" << std::endl;

    for (const auto& childGroup : children) {
        if (&childGroup != nullptr) {
            std::cout << "Drawing child group" << std::endl;
            draw_figures(childGroup, index);
        } else {
            std::cerr << "Encountered a null child group!" << std::endl;
        }
    }

    glPopMatrix();
}

      


void apply_lights() {
    std::vector<LIGHT> lights = get_lights(world);
    for (size_t i = 0; i < lights.size(); i++) {
        LIGHT l = lights[i];
        int light = get_nlight(i);
        switch (l.type) {
            case L_POINT:
                glLightfv(light, GL_POSITION, (GLfloat[]){l.l_point.posX, l.l_point.posY, l.l_point.posZ, 1.0f});
                break;
            case L_DIRECTIONAL:
                glLightfv(light, GL_POSITION, (GLfloat[]){l.l_directional.dirX, l.l_directional.dirY, l.l_directional.dirZ, 0.0f});
                break;
            case L_SPOTLIGHT:
                glLightfv(light, GL_POSITION, (GLfloat[]){l.l_spotlight.posX, l.l_spotlight.posY, l.l_spotlight.posZ, 1.0f});
                glLightfv(light, GL_SPOT_DIRECTION, (GLfloat[]){l.l_spotlight.dirX, l.l_spotlight.dirY, l.l_spotlight.dirZ});
                glLightf(light, GL_SPOT_CUTOFF, l.l_spotlight.cutoff);
                break;
            default:
                std::cerr << "Error: Unknown light type!" << std::endl;
                break;
        }
    }

    // Desabilitar as luzes restantes
    for (size_t i = lights.size(); i < 8; i++) {
        int light = get_nlight(i);
        glDisable(light);
    }

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
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    apply_lights();

    unsigned int index = 0;
    for (const auto& group : world.groups) {
        draw_figures(group, &index);
    }

    framerate();

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
    case 't':
        trajectory_on = !trajectory_on;
        break;
    case 'n':
        normais_on = !normais_on;
        break;
    
    }
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Phase4");

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
    std::vector<LIGHT> lights = get_lights(world);

    int index = 0;
    init_vbo(models, &index, lights);

    std::vector<string> textures = get_textures(world);
    for (size_t i = 0; i < textures.size(); i++) {
        //loadTexture(textures[i], &index);
    }

    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);
    glutKeyboardFunc(keyboardFunc);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    elapsed_time = glutGet(GLUT_ELAPSED_TIME);
    init_time = (1.0f * glutGet(GLUT_ELAPSED_TIME)) / 1000.0f;

    glutMainLoop();

    delete[] buffers;  // Deallocate memory for buffers array
    delete[] normalBuffers;
    delete[] texBuffers;
    
    return 0;
}