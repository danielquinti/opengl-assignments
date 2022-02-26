#include <stdio.h>
#include <math.h>
#include <GL/glut.h>
#include <vector>
#include <algorithm>

// Controls
// QAWSED change angles on the 3 axis as before
// J changes the joint
// L changes the limb
// CTRL+LEFTCLICK+Horiz.movement rotates the legs
// RIGHTCLICK activates the menu
GLint ancho = 600;
GLint alto = 600;
// The angle set is indexed by three coordinates:
// The first one corresponds to the limb
// The second one to the joint
// And the third one to the axis
GLfloat angles[4][3][3];

// Limb can take values between 0 and 3
GLint limb = 0;
// Joint can take values between 0(uppermost) and 2(lowermost)
GLint joint = 0;

// These booleans are controlled by the submenus
// The solid boolean determines which version of the cube is to be rendered
GLboolean solid = 1;
// Speed can be 0 (Still), 1(Slow) or 2 (Fast)
GLint speed = 1;

// Mouse management
GLboolean firstTime = false;
GLint mousex = 0;
void reshape(int width, int height)
{
    // The Viewport dimensions depend on the size of the window
    glViewport(0, 0, width, height);
    // The GL_PROJECTION matrix is loaded to specify the shape
    // and volume of the projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-15, 15, -15, 15, -20, 20);

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
    //The solid boolean determines which version of the cube is to be rendered
    GLenum mode;
    if (solid) mode = GL_QUADS;
    else mode = GL_LINE_LOOP;
    glColor3f(1.0f, 0.0f, 0.0f); // Rojo
    glBegin(mode);       //cara frontal
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glEnd();

    glColor3f(0.0f, 0.5f, 0.0f); // Verde
    glBegin(mode);       //cara trasera
    glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glEnd();

    glColor3f(0.0f, 0.0f, 0.5f); // Azul
    glBegin(mode);       //cara lateral izq
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glEnd();

    glColor3f(0.5f, 0.5f, 0.0f); // Amarillo
    glBegin(mode);       //cara lateral dcha
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glEnd();

    glColor3f(0.0f, 0.5f, 0.5f); // Cyan
    glBegin(mode);       //cara arriba
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glEnd();

    glColor3f(0.5f, 0.0f, 0.5f); // Magenta
    glBegin(mode);       //cara abajo
    glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glEnd();
}

void draw_hip(void) {
    glColor3f(1.0f, 1.0f, 1.0f);
    if (solid) glutSolidSphere(0.5f, 30, 30);
    else glutWireSphere(0.5, 5, 5);
}

void draw_knee(void) {
    glColor3f(1.0f, 1.0f, 1.0f);
    if (solid) glutSolidSphere(0.5f, 30, 30);
    else glutWireSphere(0.5, 5, 5);
}

void draw_ankle(void) {
    glColor3f(1.0f, 1.0f, 1.0f);
    if (solid) glutSolidSphere(0.5f, 30, 30);
    else glutWireSphere(0.5, 5, 5);
}

void draw_upper_leg(void) {
    drawCube();
}

void draw_lower_leg(void) {
    drawCube();
}

void draw_foot(void) {
    glColor3f(0.5f, 0.5f, 0.0f);
    if (solid) glutSolidTetrahedron();
    else glutWireTetrahedron();
}

//**************************************************//

// The draw_limb() function performs the necessary transformations
// on the GL_MODEL_VIEW matrix and calls the drawing functions
// The limb argument matches the index of the angles matrix where the values corresponding to said limb are stored
void draw_limb(GLint limb)
{
    // The transformations are applied before drawing the object to comply
    // with OpenGL's premultiplication
    draw_hip();
    // Applying the three angles to the upper joint pf the limb
    glRotatef(angles[limb][0][0], 1.0f, 0.0f, 0.0f);
    glRotatef(angles[limb][0][1], 0.0f, 1.0f, 0.0f);
    glRotatef(angles[limb][0][2], 0.0f, 0.0f, 1.0f);
    glTranslatef(0.0f, -1.5f, 0.0f);
    glPushMatrix();
    glScalef(1, 2, 1);
    draw_upper_leg();

    glPopMatrix();
    glTranslatef(0.0f, -1.5f, 0.0f);
    draw_knee();
    // Applying the three angles to the middle joint of the limb
    glRotatef(angles[limb][1][0], 1.0f, 0.0f, 0.0f);
    glRotatef(angles[limb][1][1], 0.0f, 1.0f, 0.0f);
    glRotatef(angles[limb][1][2], 0.0f, 0.0f, 1.0f);
    glTranslatef(0.0f, -1.5, 0.0f);
    glPushMatrix();
    glScalef(1, 2, 1);
    draw_lower_leg();

    glPopMatrix();
    glTranslatef(0.0f, -1.5, 0.0f);
    draw_ankle();
    // Applying the three angles to the lower joint of the limb
    glRotatef(angles[limb][2][0], 1.0f, 0.0f, 0.0f);
    glRotatef(angles[limb][2][1], 0.0f, 1.0f, 0.0f);
    glRotatef(angles[limb][2][2], 0.0f, 0.0f, 1.0f);
    glTranslatef(0.0f, -1.0, 0.0f);
    draw_foot();
}

void draw_torso() {
    drawCube();
}

void draw_body() {
    glLoadIdentity();
    glScalef(3, 9, 1);
    draw_torso();

    glLoadIdentity();
    glTranslatef(2.0f, 4.0f, 0.0f);
    draw_limb(0);

    glLoadIdentity();
    glTranslatef(-2.0f, 4.0f, 0.0f);
    draw_limb(1);

    glLoadIdentity();
    glTranslatef(1.0f, -5.0f, 0.0f);
    draw_limb(2);

    glLoadIdentity();
    glTranslatef(-1.0f, -5.0f, 0.0f);
    draw_limb(3);
}


void menu_1(int id) {
    switch (id) {
    case 0:
        solid = 0;
        break;
    case 1:
        solid = 1;
        break;
    }
}

void menu_2(int id) {
    speed = id;
}

void menu(int id) {
}

void create_menu()
{
    int submenu1 = glutCreateMenu(menu_1);
    glutAddMenuEntry("Solid", 1);
    glutAddMenuEntry("Wire", 0);

    int submenu2 = glutCreateMenu(menu_2);
    glutAddMenuEntry("Still", 0);
    glutAddMenuEntry("Slow", 1);
    glutAddMenuEntry("Fast", 2);

    int menu_id = glutCreateMenu(menu);
    glutAddSubMenu("Style", submenu1);
    glutAddSubMenu("Speed", submenu2);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    draw_body();
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
        // Depending on the value of limb and joint, we update the angles of a particular joint for a certain axis
    case 'q':
    case 'Q':
        angles[limb][joint][0]++;
        break;

    case 'a':
    case 'A':
        angles[limb][joint][0]--;
        break;

    case 'w':
    case 'W':
        angles[limb][joint][1]++;
        break;

    case 's':
    case 'S':
        angles[limb][joint][1]--;
        break;

    case 'e':
    case 'E':
        angles[limb][joint][2]++;
        break;

    case 'd':
    case 'D':
        angles[limb][joint][2]--;
        break;
        // Rotate among the four limbs
    case 'l':
    case 'L':
        limb = (limb + 1) % 4;
        break;
        // Rotate among the three joints within each limb
    case 'j':
    case 'J':
        joint = (joint + 1) % 3;
        break;
    }
}
void legauto(int value) {
    angles[2][0][0] += speed * value;
    angles[3][0][0] += speed * value;
    // The timer decreases as the speed increases for a more responsive experience
    glutTimerFunc(30 / (speed + 1), legauto, value);
}

void mousemotion(int x, int y) {
    if (!firstTime) {
        int delta = mousex - x;
        if (glutGetModifiers() == GLUT_ACTIVE_CTRL)
        {
            angles[2][0][1] += delta * 0.1;
            angles[3][0][1] += delta * 0.1;
        }
    }
    else {
        //The first time the mouse moves while clicked, the angle matrix is not updated
        firstTime = false;
    }
    //The actual position substitutes the previous one
    mousex = x;
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(ancho, alto);
    glutCreateWindow("Semana 3");
    create_menu();
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);
    glutKeyboardFunc(keyboard);
    glutMotionFunc(mousemotion);
    // the timer function is incorporated into the main loop
    glutTimerFunc(20, legauto, 1.0f);
    glutMainLoop();
    return 0;
}