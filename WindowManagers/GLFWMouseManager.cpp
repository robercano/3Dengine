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

void GLFWMouseManager::DisposeMouseManager()
{
	delete _mouseManager;
}

GLFWMouseManager::GLFWMouseManager(void)
{
}

GLFWMouseManager::~GLFWMouseManager(void)
{
	glfwSetMousePosCallback(NULL);
}

bool GLFWMouseManager::registerListener(MouseListener &listener)
{
	bool setCallback = false;
	if (_listeners.empty()) {
		setCallback = true;
	}
	_listeners.push_back(&listener);
	if (setCallback) {
		/* Set the callback here as setting it in the constructor leads
		 * to an infinite loop because setting the callback calls back and
		 * the _mouseManager field is still not set when the callback is called,
		 * leading to another object being created and so on */
		glfwSetMousePosCallback(mouseCallback);
	}
    return true;
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
	GLFWMouseManager::GetMouseManager()->processMouse(x, y);
}
