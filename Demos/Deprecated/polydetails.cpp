/**
 * Test for polygon details, stippling and patterns
 *
 * @author	Roberto Sosa Cano
 */

#include <math.h>
#include <OpenGL/gl.h>
#include "WindowManager.hpp"
#include "OpenGLRenderer.hpp"

int angleStep = 1;

class Square : public Object3D
{
public:
	~Square() {}

	bool init() { return true; }
	bool destroy() { return true; }
	bool render()
	{
		int angle;

		/* Polygon */
		glColor3f(0.5, 0.2, 0.8);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glBegin(GL_POLYGON);
			for (angle=0; angle<360; angle+=angleStep) {
				double rads = angle*M_PI/180.0;
				glVertex3f(cos(rads)/2-.5, sin(rads)/2+.5, 0.0);
			}
		glEnd();
		glColor3f(1.0, 0.0, 0.0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		glPointSize(4);
		glBegin(GL_POLYGON);
			for (angle=0; angle<360; angle+=angleStep) {
				double rads = angle*M_PI/180.0;
				glVertex3f(cos(rads)/2-.5, sin(rads)/2-.5, 0.0);
			}
		glEnd();
		glColor3f(0.0, 1.0, 0.0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glLineWidth(2);
		glBegin(GL_POLYGON);
			for (angle=0; angle<360; angle+=angleStep) {
				double rads = angle*M_PI/180.0;
				if (angle%(2*angleStep)) {
					glEdgeFlag(GL_TRUE);
				} else {
					glEdgeFlag(GL_FALSE);
				}
				glVertex3f(cos(rads)/2+.5, sin(rads)/2-.5, 0.0);
			}
		glEnd();

		/* Use polygon stipple for the last one */
		GLubyte mask[128];
		int i;
		for (i=0; i<128; i++) {
			mask[i] = (i/32)%2 ? 0xF0 : 0x0F;
		}
		glColor3f(0.0, 0.0, 1.0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glPolygonStipple(mask);
		glEnable(GL_POLYGON_STIPPLE);
		glBegin(GL_POLYGON);
			for (angle=0; angle<360; angle+=angleStep) {
				double rads = angle*M_PI/180.0;
				glVertex3f(cos(rads)/2+.5, sin(rads)/2+.5, 0.0);
			}
		glEnd();

		return true;
	}
};

int main(int argc, char** argv)
{
	angleStep = argc > 1 ? strtol(argv[1], NULL, 0) : 1;

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
	wmanager->createWindow(windowName, 500, 500);

	renderer->init();	// only after creating the window
	wmanager->setRenderer(renderer);

	Square square;
	renderer->addObject(&square);

	wmanager->loop();

	return 0;
}
