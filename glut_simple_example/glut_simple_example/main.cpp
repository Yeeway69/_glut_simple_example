#include <iostream>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GL/freeglut.h>
using namespace std;

using u8vec4 = glm::u8vec4;
using ivec2 = glm::ivec2;
using vec3 = glm::dvec3;

static const ivec2 WINDOWS_SIZE(512, 512);

static void draw_triangle(const u8vec4& color, const vec3& center, double size) {
	glColor4ub(color.r, color.g, color.b, color.a);
	glBegin(GL_TRIANGLES);
	glVertex3d(center.x, center.y + size, center.z);
	glVertex3d(center.x - size, center.y - size, center.z);
	glVertex3d(center.x + size, center.y - size, center.z);
	glEnd();
}

static void display_func() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	draw_triangle( u8vec4(255, 0, 0, 255), vec3(0.0, 0.0, 0.0), 0.5);
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
	glutIdleFunc(glutPostRedisplay);

	// Enter glut main loop
	glutMainLoop();

	return EXIT_SUCCESS;
}