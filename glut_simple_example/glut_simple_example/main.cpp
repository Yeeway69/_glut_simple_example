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

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, static_cast<double>(WINDOWS_SIZE.x) / WINDOWS_SIZE.y, 0.1, 100.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 2.0, 10.0, 0.0, 2.0, 0.0, 0.0, 1.0, 0.0);

	//(0.0)(1,0)(0.1)(1,1)
	//draw one quad coordinate
	vec3 a(0, 0, 0);
	vec3 b(1, 0, 0);
	vec3 c(0, 1, 0);
	vec3 d(1, 1, 0);


	static double angle = 0.0;
	angle += 0.1;

	glRotated(angle, 0.0, 1.0, 0.0);

	glBegin(GL_TRIANGLES);
	

	
	glColor3ub(200, 20, 0);
	glVertex3d(a.x, a.y, a.z);
	glVertex3d(b.x, b.y, b.z);
	glVertex3d(c.x, c.y, c.z);

	glColor3ub(0, 20, 200);
	glVertex3d(b.x, b.y, b.z);
	glVertex3d(c.x, c.y, c.z);
	glVertex3d(d.x, d.y, d.z);

	glEnd();

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