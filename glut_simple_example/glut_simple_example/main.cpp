#include <iostream>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GL/freeglut.h>
using namespace std;

using ivec2 = glm::ivec2;
using vec3 = glm::vec3;

static const ivec2 WINDOWS_SIZE(512, 512);

static void display_func() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glutSwapBuffers();
}

static void init_opengl() {
	glewInit();
	glClearColor(0.5, 0.5, 0.5, 1.0);
}

int main(int argc, char* argv[]) {
	// Iniit window and context
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(WINDOWS_SIZE.x, WINDOWS_SIZE.y);
	glutCreateWindow("Glut Simple Example");

	// Init OpenGL
	init_opengl();

	// Set Glut callbacks
	glutDisplayFunc(display_func);

	// Enter glut main loop
	glutMainLoop();

	return EXIT_SUCCESS;
}