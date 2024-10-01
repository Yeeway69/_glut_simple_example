#include <iostream>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "Camera.h"
using namespace std;

struct Triangle {
	Transform transform;
	glm::u8vec3 color;
	double size = 0.5;

	void draw() const {
		glPushMatrix();
		glMultMatrixd(&transform.mat()[0][0]);
		glColor3ub(color.r, color.g, color.b);
		glBegin(GL_TRIANGLES);
		glVertex2d( 0, size);
		glVertex2d( - size, - size);
		glVertex2d(size, - size);
		glEnd();
		glPopMatrix();
	}
};

static Camera camera;
static Triangle red_triangle;
static Triangle green_triangle;
static Triangle blue_triangle;

static void drawFloorGrid(int size, double step) {
	glColor3ub(0, 0, 0);
	glBegin(GL_LINES);
	for (double i = -size; i <= size; i += step) {
		glVertex3d(i, 0, -size);
		glVertex3d(i, 0, size);
		glVertex3d(-size, 0, i);
		glVertex3d(size, 0, i);
	}
	glEnd();
}

static void display_func() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixd(&camera.view()[0][0]);

	drawFloorGrid(16, 0.25);
	red_triangle.draw();
	green_triangle.draw();
	blue_triangle.draw();

	glutSwapBuffers();
}

static void init_opengl() {
	glewInit();

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);

	glEnable(GL_POLYGON_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POINT_SMOOTH);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	glEnable(GL_DEPTH_TEST);

	glClearColor(0.5, 0.5, 0.5, 1.0);
}

static void reshape_func(int width, int height) {
	glViewport(0, 0, width, height);
	camera.aspect = static_cast<double>(width) / height;
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixd(&camera.projection()[0][0]);
}

static void mouseWheel_func(int wheel, int direction, int x, int y) {
	camera.transform().translate( vec3(0, 0, direction * 0.1));
}

static void idle_func() {
	//animate triangles
	red_triangle.transform.rotate(0.001, vec3(0, 1, 0));
	green_triangle.transform.rotate(0.001, vec3(1, 0, 0));
	blue_triangle.transform.rotate(0.001, vec3(0, 0, 1));
	glutPostRedisplay();
}

int main(int argc, char* argv[]) {
	// Iniit window and context
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(1280, 720);
	glutCreateWindow("Glut Simple Example");

	// Init OpenGL
	init_opengl();

	// Init camera
	camera.transform().pos() = vec3(0, 1, 4);
	camera.transform().rotate(glm::radians(180.0), vec3(0, 1, 0));

	// Init triangles
	red_triangle.transform.pos() = vec3(0, 1, 0);
	red_triangle.color = glm::u8vec3(255, 0, 0);
	green_triangle.transform.pos() = vec3(1, 1, 0);
	green_triangle.color = glm::u8vec3(0, 255, 0);
	blue_triangle.transform.pos() = vec3(0, 1, 1);
	blue_triangle.color = glm::u8vec3(0, 0, 255);

	// Set Glut callbacks
	glutDisplayFunc(display_func);
	glutIdleFunc(idle_func);
	glutReshapeFunc(reshape_func);
	glutMouseWheelFunc(mouseWheel_func);

	// Enter glut main loop
	glutMainLoop();

	return EXIT_SUCCESS;
}