#include <stdio.h>
#include <math.h>
#include <GL/glut.h>

GLint ancho = 400;
GLint alto = 400;

//The angles for the two moving parts are initialized
// Toggle determines which bodypart the user is moving
// By default, we assume he wants to move the upperLeg
GLboolean toggle = 1;
GLfloat upperLegAngleX = 0.0f;
GLfloat upperLegAngleY = 0.0f;
GLfloat upperLegAngleZ = 0.0f;
GLfloat lowerLegAngleX = 0.0f;
GLfloat lowerLegAngleY = 0.0f;
GLfloat lowerLegAngleZ = 0.0f;

void reshape(int width, int height)
{
    // The Viewport dimensions depend on the size of the window
    glViewport(0, 0, width, height);
    // The GL_PROJECTION matrix is loaded to specify the shape
    // and volume of the projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-10, 10, -10, 10, -20, 20);

    // The GL_MODELVIEW matrix is loaded to draw the objects.
    glMatrixMode(GL_MODELVIEW);

    ancho = width;
    alto = height;
}
//***************************************************//
// These functions determine the shape and colour of
// each bodypart, but they don't perform any
// transformations of the GL_MODELVIEW matrix
void drawCube(void)
{
    glColor3f(1.0f, 0.0f, 0.0f); // Rojo
    glBegin(GL_QUADS);       //cara frontal
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glEnd();

    glColor3f(0.0f, 0.5f, 0.0f); // Verde
    glBegin(GL_QUADS);       //cara trasera
    glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glEnd();

    glColor3f(0.0f, 0.0f, 0.5f); // Azul
    glBegin(GL_QUADS);       //cara lateral izq
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glEnd();

    glColor3f(0.5f, 0.5f, 0.0f); // Amarillo
    glBegin(GL_QUADS);       //cara lateral dcha
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glEnd();

    glColor3f(0.0f, 0.5f, 0.5f); // Cyan
    glBegin(GL_QUADS);       //cara arriba
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glEnd();

    glColor3f(0.5f, 0.0f, 0.5f); // Magenta
    glBegin(GL_QUADS);       //cara abajo
    glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glEnd();
}

void draw_hip(void) {
    glColor3f(1.0f, 1.0f, 1.0f);
    glutSolidSphere(0.5f, 30, 30);
}

void draw_knee(void) {
    glColor3f(1.0f, 1.0f, 1.0f);
    glutSolidSphere(0.5f, 30, 30);
}

void draw_upper_leg(void) {
    drawCube();
}

void draw_lower_leg(void) {
    drawCube();
}
//**************************************************//

// The draw_leg() function performs the necessary transformations
// on the GL_MODEL_VIEW matrix and calls the drawing functions
void draw_leg()
{
    glLoadIdentity();
    draw_hip();
    // The transformations are applied before drawing the object to comply
    // with OpenGL's premultiplication
    glRotatef(upperLegAngleX, 1.0f, 0.0f, 0.0f);
    glRotatef(upperLegAngleY, 0.0f, 1.0f, 0.0f);
    glRotatef(upperLegAngleZ, 0.0f, 0.0f, 1.0f);
    glTranslatef(0.0f, -1.0f, 0.0f);
    draw_upper_leg();

    glTranslatef(0.0f, -1.0f, 0.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    draw_knee();

    glRotatef(lowerLegAngleX, 1.0f, 0.0f, 0.0f);
    glRotatef(lowerLegAngleY, 0.0f, 1.0f, 0.0f);
    glRotatef(lowerLegAngleZ, 0.0f, 0.0f, 1.0f);
    glTranslatef(0.0f, -1.0, 0.0f);

    draw_lower_leg();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    draw_leg();
    glutSwapBuffers();
}

void init()
{
    glClearColor(0, 0, 0, 0);
    glEnable(GL_DEPTH_TEST);
    ancho = 400;
    alto = 400;
}


void idle()
{
    display();
}

// The keyboard event handler updates the desired angle 
// of the desired bodypart on command
void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    // Uppercase and lowercase keys are considered equivalent
    case 'q':
    case 'Q':
        // Toggle specifies which bodypart to move
        if (toggle) upperLegAngleX++;
        else lowerLegAngleX++;
        break;

    case 'a':
    case 'A':
        if (toggle) upperLegAngleX--;
        else lowerLegAngleX--;
        break;

    case 'w':
    case 'W':
        if (toggle) upperLegAngleY++;
        else lowerLegAngleY++;
        break;

    case 's':
    case 'S':
        if (toggle) upperLegAngleY--;
        else lowerLegAngleY--;
        break;

    case 'e':
    case 'E':
        if (toggle) upperLegAngleZ++;
        else lowerLegAngleZ++;       
        break;

    case 'd':
    case 'D':
        if (toggle) upperLegAngleZ--;
        else lowerLegAngleZ--;
        break;
    // By pressing T, we switch between the upper
    // and the lower bodypart
    case 't':
    case 'T':
        if (toggle) toggle = 0;
        else toggle = 1;
    }
}


int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(ancho, alto);
    glutCreateWindow("Semana 3");
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}