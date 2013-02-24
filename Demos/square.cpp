/**
 * Plain simple square test
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
		glColor3f(1.0, 0.0, 0.0);
		glBegin(GL_POLYGON);
			glVertex3f(0.25, 0.25, 0.0);
			glVertex3f(0.75, 0.25, 0.0);
			glVertex3f(0.75, 0.75, 0.0);
			glVertex3f(0.25, 0.75, 0.0);
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
