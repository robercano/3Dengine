/**
 * Playing around with different primitives details
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

	bool render()
	{
		printf("Rendering!\n");
		glClearColor( 0.0f, 0.0f, 0.0f, 0.0f ); //clear background screen to black

		//Clear information from last draw
		glClear( GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_MODELVIEW); //Switch to the drawing perspective
		glLoadIdentity(); //Reset the drawing perspective

		glBegin(GL_QUADS); //Begin quadrilateral coordinates

		//Trapezoid
		glVertex3f(-0.7f, -1.5f, -5.0f);
		glVertex3f(0.7f, -1.5f, -5.0f);
		glVertex3f(0.4f, -0.5f, -5.0f);
		glVertex3f(-0.4f, -0.5f, -5.0f);

		glEnd(); //End quadrilateral coordinates

		glBegin(GL_TRIANGLES); //Begin triangle coordinates

		//Pentagon
		glVertex3f(0.5f, 0.5f, -5.0f);
		glVertex3f(1.5f, 0.5f, -5.0f);
		glVertex3f(0.5f, 1.0f, -5.0f);

		glVertex3f(0.5f, 1.0f, -5.0f);
		glVertex3f(1.5f, 0.5f, -5.0f);
		glVertex3f(1.5f, 1.0f, -5.0f);

		glVertex3f(0.5f, 1.0f, -5.0f);
		glVertex3f(1.5f, 1.0f, -5.0f);
		glVertex3f(1.0f, 1.5f, -5.0f);

		//Triangle
		glVertex3f(-0.5f, 0.5f, -5.0f);
		glVertex3f(-1.0f, 1.5f, -5.0f);
		glVertex3f(-1.5f, 0.5f, -5.0f);

		glEnd(); //End triangle coordinates
		return true;
	}
};

int main(int argc, char** argv)
{
	WindowManager *wmanager = WindowManager::GetWindowManager(WindowManager::WINDOW_MANAGER_GLFW);
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
	wmanager->setRenderer(renderer);

	/* Set the window size */
	std::string windowName("OpenGL Test");
	wmanager->createWindow(windowName, 512, 512);

//	renderer->init();	// only after creating the window

	Square square;
	renderer->addObject(&square);
	wmanager->loop();

	return 0;
}
