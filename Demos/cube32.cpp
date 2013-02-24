/*
 * Vertex Buffer Objects demo
 *
 * @author	Roberto Sosa Cano
 */
#include <stdio.h>
#include "WindowManager.hpp"
#include "OpenGLRenderer.hpp"
#include "ObjectLib.hpp"

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

	/* Set the window size */
	std::string windowName("OpenGL Test");
	wmanager->createWindow(windowName, 1024, 768);

	renderer->init();	// only after creating the window
	wmanager->setRenderer(renderer);

	ColorCube cube;
	renderer->addObject(&cube);

	wmanager->loop();
	return 0;
}
