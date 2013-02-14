/**
 * Engine main
 *
 * @author	Roberto Sosa Cano
 */

#include "WindowManagers/WindowManager.hpp"
#include "Renderers/OpenGLRenderer.hpp"

int main(int argc, char** argv)
{
	WindowManager *wmanager = WindowManager::NewWindowManager(WindowManager::WINDOW_MANAGER_GLUT);
	if (!wmanager) {
		fprintf(stderr, "ERROR creating new window manager\n");
		exit(1);
	}

	Renderer *renderer = new OpenGLRenderer();
	if (!renderer) {
		fprintf(stderr, "ERROR allocating renderer\n");
		WindowManager::DeleteWindowManager(wmanager);
		exit(1);
	}

	/* Init the window manager and the render*/
	wmanager->init();

	/* Set the window size */
	std::string windowName("OpenGL Test");
	wmanager->createWindow(windowName, 1024, 768);

	renderer->init();	// only after creating the window
	wmanager->setRenderer(renderer);

	Object3D object;
	renderer->addObject(&object);

	wmanager->loop();

	return 0;
}
