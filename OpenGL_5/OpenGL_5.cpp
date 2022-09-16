#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include <math.h>
#include <glm/glm.hpp>

#define initWidth 500
#define initHeight 500

GLdouble eyeX = 0, eyeY = 0.2, eyeZ = 0.5;
GLdouble centerX = 0, centerY = 0, centerZ = 0;
GLdouble upX = 0, upY = 1, upZ = 0;
GLdouble radios = 0.5;
GLfloat temp;

GLfloat angle_Upper = 20;
GLfloat angle_Lower = 20;

double camera_angle_h = 0;
double camera_angle_v = 0;
int drag_x_origin;
int drag_y_origin;
int dragging = 0;



void EnableLighting(void)
{
	/* Control material properties */
	GLfloat mat_specular[] = { 0.7, 0.0, 0.0, 1.0 };
	GLfloat mat_diffuse[] = { 0.5, 0.0, 0.0, 1.0 };
	GLfloat mat_ambient[] = { 0.5, 0.0, 0.0, 1.0 };
	GLfloat mat_shininess = { 7.0 };

	/* Control lighting properties */
	GLfloat light_ambient[] = { .5, .5, .5, 1.0 };
	GLfloat light_diffuse[] = { .5, .5, .5, 1.0 };
	GLfloat light_specular[] = { .7, .7, .7, 1.0 };
	GLfloat light_position[] = { 100.0, 50.0, 120.0, 1.0 };

	/* set up ambient, diffuse, and specular components for light 0 */
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	/* define material properties for front face of all polygons */
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);

	glEnable(GL_SMOOTH);     // enable smooth shading 
	glEnable(GL_LIGHTING);   // enable lighting 
	glEnable(GL_LIGHT0);     // enable light 0 
}



void drawArrow() {
	glBegin(GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(0.5, 0, 0);

	glVertex3f(0.5, 0, 0);
	glVertex3f(0.3, 0, -0.1);

	glVertex3f(0.5, 0, 0);
	glVertex3f(0.3, 0, 0.1);
	glEnd();
}

void drawOrtho() {
	glPushMatrix();
	glColor3f(1, 0, 0); // x
	drawArrow();

	glPushMatrix();
	glColor3f(0, 1, 0); // z
	glRotated(-90, 0, 1, 0);
	drawArrow();
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0, 1); // y
	glRotated(-45, 0, 1, 0);
	glRotated(90, 0, 0, 1);
	drawArrow();
	glPopMatrix();

	glColor3f(0, 0, 0);
	glPopMatrix();
}

void drawCube(float x, float y, float z) {

	glPushMatrix();
	glScalef(x, y, z);
	glColor3f(1, 0, 0);
	glutSolidCube(1);
	glPopMatrix();
}

void drawBody() {

	//glColor3f(1, 0, 0);
	drawCube(0.3, 0.6, 0.15);

}

void drawUpperArm(GLfloat angle) {
	glTranslatef(0.15, 0.25, 0);
	glRotatef(angle, 0, 0, 1);
	glTranslatef(0.15, 0, 0);
	//glColor3f(0, 1, 0);
	drawCube(0.3, 0.13, 0.13);
}

void drawLowerArm(GLfloat angle) {
	glTranslatef(0.15, 0, 0);
	glRotatef(angle, 0, 0, 1);
	glTranslatef(0.15, 0, 0);
	//glColor3f(0, 0, 1);
	drawCube(0.3, 0.13, 0.13);
}

void drawHand() {

	glTranslatef(0.17, 0, 0);
	glPushMatrix();
	//glColor3f(1, 1, 0);
	glutSolidSphere(0.08, 20, 20);
	glPopMatrix();
}

void drawRobot() {
	drawBody();
	drawUpperArm(angle_Upper);
	drawLowerArm(angle_Lower);
	drawHand();
}

void MyDisplay() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST);
	glLoadIdentity();
	gluLookAt(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ);

	glRotated(camera_angle_v, 1.0, 0.0, 0.0);
	glRotated(camera_angle_h, 0.0, 1.0, 0.0);

	drawRobot();

	glutSwapBuffers();
}

void mouse(GLint button, GLint state, GLint x, GLint y)
{
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) {
			dragging = 1;
			drag_x_origin = x;
			drag_y_origin = y;
		}
		else
			dragging = 0;
	}


}


void motion(GLint x, GLint y)
{
	if (dragging) {
		camera_angle_v += (y - drag_y_origin) * 0.3;
		camera_angle_h += (x - drag_x_origin) * 0.3;
		drag_x_origin = x;
		drag_y_origin = y;
	}

}



void MyReshape(int width, int height) {

	glViewport(0, 0, width, height); // 사용할 화면 크기 설정
	GLfloat widthFactor = (GLfloat)width / (GLfloat)initWidth; // 초기화면에서의 변한 비율
	GLfloat heightFactor = (GLfloat)height / (GLfloat)initHeight; // 초기화면에서의 변한 비율

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.0 * widthFactor, 1.0 * widthFactor,
		-1.0 * heightFactor, 1.0 * heightFactor, -2.0, 2.0);
}

void MyTimer(int value) {

	angle_Upper += 1;
	angle_Lower -= 2;

	glutPostRedisplay();
	glutTimerFunc(20, MyTimer, 0);
}



int main(int argc, char* argv[]) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(500, 500);
	glutCreateWindow("GL Study");
	EnableLighting();
	glClearColor(1, 1, 1, 1);
	glutDisplayFunc(MyDisplay);
	glutReshapeFunc(MyReshape);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutTimerFunc(20, MyTimer, 0);

	glutMainLoop();
	return 0;
}
