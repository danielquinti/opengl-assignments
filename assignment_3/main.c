#include <stdio.h>
#include <math.h>
#include <GL/glut.h>
#include <vector>
#include <algorithm>

GLint ancho = 600;
GLint alto = 600;


GLint plateangle = 0;
GLfloat baseanglez = 0;
GLfloat baseanglex = -90;
GLfloat armangle = 0;
GLint speed = 1;
GLint direction = -1;
GLboolean projection = 0;
GLint stop = 0;
GLboolean firstTime = 1;
GLint mouseposx = 0;
GLint mouseposy = 0;
GLfloat colour[3];
GLint platespeed = 1;
GLint updown = 1;
GLint armheight = 4;
GLfloat basewidth = 7;
GLfloat baseheight = 2;

void reshape(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (projection)
        gluPerspective(90.0f, (GLfloat)width / (GLfloat)height, 1.0f, 30.0f);
    else
        glOrtho(-15, 15, -15, 15, -20, 40);
    glMatrixMode(GL_MODELVIEW);

    ancho = width;
    alto = height;
}
void draw_circle(float radius, int segments) {
    glBegin(GL_POLYGON);
    for (int ii = 0; ii < segments; ii++) {
        float theta = 2.0f * 3.1415926f * float(ii) / float(segments);
        float x = radius * cosf(theta);
        float y = radius * sinf(theta);
        glVertex3f(x, y, 0);
    }
    glEnd();
}


void draw_cups_base() {
    glColor3f(1.0f, 1.0f, 1.0f);
    gluCylinder(gluNewQuadric(), basewidth, basewidth, baseheight, 30, 30);
    draw_circle(basewidth, 30);
    glTranslatef(0, 0, baseheight);
    draw_circle(basewidth, 30);
    glTranslatef(0, 0, -baseheight);
}

void draw_cups_mount() {
    glColor3f(0.0f, 0.0f, 1.0f);
    glutSolidCone(basewidth, baseheight, 30, 30);
}

//the arm is a scaled cube
void draw_ferris_arm() {
    glPushMatrix();
    glScalef(armheight, 1.0f, 1.0f);
    glColor3f(1.0f, 0.0f, 0.0f);
    glutSolidCube(1);
    glPopMatrix();
}

//the cubes are moved to fit on the platform
void drawpassenger(float x, float y) {
    glPushMatrix();
    glTranslatef(x, y, -0.5f);
    glColor3f(colour[0], colour[1], colour[2]);
    glutSolidCube(1.0f);
    glPopMatrix();
}

//the cone needs to be rotated so its apex points upwards
void drawplatform() {
    glRotatef(armangle, 0.0f, 1.0f, 0.0f);
    glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
    glRotatef(baseanglez + plateangle, 0.0f, 0.0f, 1.0f);
    glTranslatef(0.0f, 0.0f, -1.0f);
    glColor3f(0, 1, 0);
    glutSolidCone(armheight, 1, 16, 16);
}

void draw_cups() {
    glLoadIdentity();
    glTranslatef(0, 0, -15);
    glRotatef(baseanglex, 1.0f, 0.0f, 0.0f);
    glRotatef(baseanglez, 0.0f, 0.0f, 1.0f);
    draw_cups_base();
    glTranslatef(0, 0, baseheight);
    draw_cups_mount();

    glTranslatef(0.0, 0.0, 2.0);
    glRotatef(-armangle, 0.0f, 1.0f, 0.0f);
    glTranslatef(armheight / 2, 0.0f, 0.0f);

    draw_ferris_arm();

    glTranslatef(armheight / 2, 0.0f, 0.0f);

    drawplatform();
    drawpassenger(2.0, -2.0);
    drawpassenger(-2.0, -2.0);
    drawpassenger(-2.0, 2.0);
    drawpassenger(2.0, 2.0);


}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    draw_cups();
    glutSwapBuffers();
}

void init()
{
    glClearColor(0, 0, 0, 0);
    colour[0] = 1;
    colour[1] = 0;
    colour[2] = 0;
    glEnable(GL_DEPTH_TEST);
    ancho = 400;
    alto = 400;
}


void idle()
{
    display();
}
//Reused code from last week
void craneauto(int value) {
    baseanglez = fmod(baseanglez + (direction * speed * value), 360);
    plateangle = fmod(plateangle + (direction * speed * value), 360);
    if (armangle >= 45)
        updown = -1;
    else
        if (armangle <= 0)
            updown = 1;
    armangle += updown * speed;
    glutTimerFunc(20 / (speed + 1), craneauto, value);
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'p':
    case 'P':
        projection = 1;
        reshape(ancho, alto);
        break;

    case 'o':
    case 'O':
        projection = 0;
        reshape(ancho, alto);
        break;

    case 'r':
    case 'R':
        direction *= -1;
        break;

    case 's':
    case 'S':
        speed = 0;
        break;
    }
}
//Reused code from last week
void mousemotion(int x, int y) {
    if (!firstTime) {
        int deltaposy = mouseposy - y;
        int deltaposx = mouseposx - x;

        if (glutGetModifiers() == GLUT_ACTIVE_ALT)
            baseanglex += deltaposx;
        else
            armangle += deltaposx;

    }
    else {
        firstTime = false;
    }
    mouseposx = x;
    mouseposy = y;
}

void menu_1(int id) {
    switch (id) {
    case 0:
        colour[0] = 1;
        colour[1] = 1;
        colour[2] = 1;
        break;
    case 1:
        colour[0] = 0;
        colour[1] = 0;
        colour[2] = 1;
        break;
    case 2:
        colour[0] = 0;
        colour[1] = 1;
        colour[2] = 0;
        break;
    case 3:
        colour[0] = 1;
        colour[1] = 0;
        colour[2] = 0;
        break;
    }
}

//Reused code from last week
void menu_2(int id) {
    speed = id;
}

void menu(int id) {
}

void create_menu()
{
    int submenu1 = glutCreateMenu(menu_1);
    glutAddMenuEntry("White", 0);
    glutAddMenuEntry("Blue", 1);
    glutAddMenuEntry("Green", 2);
    glutAddMenuEntry("Red", 3);


    int submenu2 = glutCreateMenu(menu_2);
    glutAddMenuEntry("Still", 0);
    glutAddMenuEntry("Slow", 1);
    glutAddMenuEntry("Fast", 2);

    int menu_id = glutCreateMenu(menu);
    glutAddSubMenu("Colour", submenu1);
    glutAddSubMenu("Speed", submenu2);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(ancho, alto);
    glutCreateWindow("Examen");
    create_menu();
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);
    glutKeyboardFunc(keyboard);
    glutPassiveMotionFunc(mousemotion);
    glutTimerFunc(20, craneauto, 1.0f);
    glutMainLoop();
    return 0;
}