#include <iostream>
#include <array>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <IL/il.h>
#include <IL/ilu.h>
#include "Camera.h"
#include <vector>
#include "Mesh.h"
using namespace std;

struct Triangle {
	Transform transform;
	glm::u8vec3 color;
	Mesh mesh;

	void initMesh(double size) {
		const glm::vec3 vertices[] = {
			glm::vec3(-size, -size, 0),
			glm::vec3(size, -size, 0),
			glm::vec3(0, size, 0)
		};

		glm::u8vec3 colors[] = {
			glm::u8vec3(255, 0, 0),
			glm::u8vec3(0, 255, 0),
			glm::u8vec3(0, 0, 255)
		};
		const unsigned int indices[] = { 0, 1, 2 };
		mesh.load(vertices, 3, indices, 3);
		mesh.loadColors(colors, 3);
	}

	void draw()
	{
		glPushMatrix();
		glMultMatrixd(&transform.mat()[0][0]);
		glColor3ub(color.r, color.g, color.b);
		mesh.draw();
		glPopMatrix();
	}

};



struct Cube
{
    Transform transform;
	glm::u8vec3 color = glm::u8vec3(100, 255, 150);

	vector<vec3> vertex_data = 
	{
		vec3(-1, -1, -1),
		vec3(1, -1, -1),
		vec3(1, 1, -1),
		vec3(-1, 1, -1),
		vec3(-1, -1, 1),
		vec3(1, -1, 1),
		vec3(1, 1, 1),
		vec3(-1, 1, 1)
	};

	


    vector<unsigned int> index_data =
	{
		0, 1, 2, 2, 3, 0,
		1, 5, 6, 6, 2, 1,
		7, 6, 5, 5, 4, 7,
		4, 0, 3, 3, 7, 4,
		3, 2, 6, 6, 7, 3,
		4, 5, 1, 1, 0, 4
	};

	unsigned int vertex_data_butter_id = 0;
	unsigned int index_data_butter_id = 0;

	void initBuffers() {

		vec3 vertex[123];
		sizeof(vertex);

		glGenBuffers(1, &vertex_data_butter_id);
		glBindBuffer(GL_ARRAY_BUFFER, vertex_data_butter_id);
		glBufferData(GL_ARRAY_BUFFER, vertex_data.size() * sizeof(vec3), &vertex_data[0], GL_STATIC_DRAW);

		glGenBuffers(1, &index_data_butter_id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_data_butter_id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_data.size() * sizeof(unsigned int), &index_data[0], GL_STATIC_DRAW);
	}

    void draw() const {
        glPushMatrix();
        glMultMatrixd(&transform.mat()[0][0]);
        glColor3ub(color.r, color.g, color.b);
        glEnableClientState(GL_VERTEX_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, vertex_data_butter_id);
		glVertexPointer(3, GL_DOUBLE, 0, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_data_butter_id);
		glDrawElements(GL_TRIANGLES, index_data.size(), GL_UNSIGNED_INT, 0);
		glDisableClientState(GL_VERTEX_ARRAY);
        glPopMatrix();
    }
};


static Camera camera;
static Triangle red_triangle;
static Triangle green_triangle;
static Triangle blue_triangle;

static Cube cube;

static array< array<glm::u8vec3, 256>, 256> texture;


static void initializeTexture()
{
	for (int s = 0; s < texture.size(); ++s)
	{
		for (int t = 0; t < texture.size(); t++)
		{
			const glm::u8vec3 whiteColor(255, 255, 255);
			const glm::u8vec3 blackColor(0, 0, 0);
			texture[s][t] = (s + t) % 2 == 0 ? whiteColor : blackColor;
		}
	}
}


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

	/*cube.draw();*/

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
	red_triangle.transform.rotate(0.0001, vec3(0, 1, 0));
	green_triangle.transform.rotate(0.0001, vec3(1, 0, 0));
	blue_triangle.transform.rotate(0.0001, vec3(0, 0, 1));

	cube.transform.rotate(0.001, vec3(0.2, 0.5, 0.1));
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
	red_triangle.initMesh(1);


	// Init texture

	ilInit();
	iluInit();
	auto il_img_id = ilGenImage();
	ilBindImage(il_img_id);
	ilLoadImage("Lenna.jpeg");
	auto img_width = ilGetInteger(IL_IMAGE_WIDTH);
	auto img_height = ilGetInteger(IL_IMAGE_HEIGHT);
	auto img_bpp = ilGetInteger(IL_IMAGE_BPP);
	auto img_format = ilGetInteger(IL_IMAGE_FORMAT);
	auto img_data = ilGetData();


	
	// Init cube
	cube.initBuffers();

	// Set Glut callbacks
	glutDisplayFunc(display_func);
	glutIdleFunc(idle_func);
	glutReshapeFunc(reshape_func);
	glutMouseWheelFunc(mouseWheel_func);

	// Enter glut main loop
	glutMainLoop();

	return EXIT_SUCCESS;
}