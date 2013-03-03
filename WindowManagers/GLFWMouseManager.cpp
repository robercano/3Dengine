/**
 * @class	GLFWMouseManager
 * @brief	GLFW based mouse manager
 *
 * @author	Roberto Sosa Cano
 */
#include <GL/glfw.h>
#include "GLFWMouseManager.hpp"

GLFWMouseManager *GLFWMouseManager::_mouseManager = NULL;

GLFWMouseManager *GLFWMouseManager::GetMouseManager(void)
{
	if (_mouseManager == NULL) {
		_mouseManager = new GLFWMouseManager();
	}
	return _mouseManager;
}

GLFWMouseManager::GLFWMouseManager(void)
{
	glfwSetMousePosCallback(mouseCallback);
}

bool GLFWMouseManager::registerListener(MouseListener &listener)
{
	_listeners.push_back(&listener);
}

void GLFWMouseManager::processMouse(int32_t x, int32_t y)
{
	std::vector<MouseListener *>::iterator listener;
	for (listener = _listeners.begin(); listener != _listeners.end(); ++listener) {
		(*listener)->processMouse(x, y);
	}
}

void GLFWMouseManager::mouseCallback(int x, int y)
{
	printf("(%d, %d)\n", x, y);
	GLFWMouseManager::GetMouseManager()->processMouse(x, y);
}
