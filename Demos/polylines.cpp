/**
 * Test for the different types of primitives supported by glBegin/glEnd
 *
 * @author	Roberto Sosa Cano
 */

#include <OpenGL/gl.h>
#include "WindowManager.hpp"
#include "OpenGLRenderer.hpp"

class Square : public Object3D
{
public:
	~Square() {}

	bool init() {}
	bool destroy() {}
	bool render()
	{
		/* Points */
		glColor3f(1.0, 0.0, 0.0);
		glBegin(GL_POINTS);
			glVertex3f(0.1, 0.1, 0.0);
			glVertex3f(0.2, 0.1, 0.0);
			glVertex3f(0.1, 0.2, 0.0);
			glVertex3f(0.2, 0.2, 0.0);
		glEnd();

		/* Lines */
		glColor3f(0.0, 1.0, 0.0);
		glBegin(GL_LINES);
			glVertex3f(0.3, -0.1, 0.0);
			glVertex3f(0.4, -0.2, 0.0);
			glVertex3f(0.3, -0.3, 0.0);
			glVertex3f(0.4, -0.4, 0.0);
		glEnd();

		/* Lines */
		glColor3f(0.0, 0.0, 1.0);
		glBegin(GL_LINE_STRIP);
			glVertex3f(-1.0, -1.0, 0.0);
			glVertex3f(-0.8, -0.8, 0.0);
			glVertex3f(-0.7, -0.5, 0.0);
			glVertex3f(-0.2,  0.0, 0.0);
		glEnd();

		/* Line loop */
		glColor3f(0.0, 1.0, 1.0);
		glBegin(GL_LINE_LOOP);
			glVertex3f(1.0, 1.0, 0.0);
			glVertex3f(0.8, 0.8, 0.0);
			glVertex3f(0.7, 0.5, 0.0);
			glVertex3f(0.2,  0.0, 0.0);
		glEnd();

		/* Triangle */
		glColor3f(0.5, 0.2, 0.8);
		glBegin(GL_TRIANGLES);
			glVertex3f(-0.5, 0.5, 0.0);
			glVertex3f( 0.0, 0.5, 0.0);
			glVertex3f(-.25, 0.0, 0.0);
		glEnd();

		/* Triangle strip */
		glColor3f(0.3, 0.3, 0.2);
		glBegin(GL_TRIANGLE_STRIP);
			glVertex3f(0.2, 0.8, 0.0);
			glVertex3f(0.4, 0.8, 0.0);
			glVertex3f(0.3, 0.7, 0.0);
			glVertex3f(0.5, 0.6, 0.0);
			glVertex3f(0.4, 0.3, 0.0);
		glEnd();

		/* Triangle fan */
		glColor3f(0.5, 0.7, 0.9);
		glBegin(GL_TRIANGLE_FAN);
			glVertex3f(0.0, -0.5, 0.0);
			glVertex3f(0.2, -0.5, 0.0);
			glVertex3f(-0.1, -0.6, 0.0);
			glVertex3f(-0.2, -0.3, 0.0);
		glEnd();

		/* Quads */
		glColor3f(0.9, 0.2, 0.9);
		glBegin(GL_QUADS);
			glVertex3f(-0.9, -0.2, 0.0);
			glVertex3f(-0.8, -0.2, 0.0);
			glVertex3f(-0.8, -0.4, 0.0);
			glVertex3f(-0.9, -0.4, 0.0);
		glEnd();

		/* Quad strip */
		glColor3f(0.2, 0.9, 0.5);
		glBegin(GL_QUAD_STRIP);
			glVertex3f(-0.7, -0.6, 0.0);
			glVertex3f(-0.73, -0.55, 0.0);
			glVertex3f(-0.72, -0.6, 0.0);
			glVertex3f(-0.6, -0.62, 0.0);
			glVertex3f(-0.72, -0.70, 0.0);
			glVertex3f(-0.6, -0.70, 0.0);
		glEnd();

		/* Polygon */
		glColor3f(0.33, 0.44, 0.55);
		glBegin(GL_POLYGON);
			glVertex3f(0.2, 0.2, 0.0);
			glVertex3f(0.3, 0.3, 0.0);
			glVertex3f(0.32, 0.4, 0.0);
			glVertex3f(0.3, 0.5, 0.0);
			glVertex3f(0.2, 0.6, 0.0);
			glVertex3f(0.1, 0.6, 0.0);
		glEnd();
	}
};

int main(int argc, char** argv)
{
	WindowManager *wmanager = WindowManager::GetWindowManager(WindowManager::WINDOW_MANAGER_GLUT);
	if (!wmanager) {
		fprintf(stderr, "ERROR creating new window manager\n");
		exit(1);
	}

	Renderer *renderer = new OpenGLRenderer();
	if (!renderer) {
		fprintf(stderr, "ERROR allocating renderer\n");
		WindowManager::DisposeWindowManager(wmanager);
		exit(1);
	}

	/* Init the window manager and the render*/
	wmanager->init();

	/* Set the window size */
	std::string windowName("OpenGL Test");
	wmanager->createWindow(windowName, 1024, 768);

	renderer->init();	// only after creating the window
	wmanager->setRenderer(renderer);

	Square square;
	renderer->addObject(&square);

	wmanager->loop();

	return 0;
}
