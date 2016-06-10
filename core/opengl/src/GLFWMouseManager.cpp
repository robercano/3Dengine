/**
 * @class	GLFWMouseManager
 * @brief	GLFW based mouse manager
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#include "OpenGL.h"
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
	glfwSetCursorPosCallback(glfwGetCurrentContext(), NULL);
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
		glfwSetCursorPosCallback(glfwGetCurrentContext(), mouseCallback);

        double xPos, yPos;
        glfwGetCursorPos(glfwGetCurrentContext(), &xPos, &yPos);

        /* Call the callback manually to initialize the values */
        listener.processMouse((int32_t)xPos, (int32_t)yPos);
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

void GLFWMouseManager::mouseCallback(GLFWwindow *w, double x, double y)
{
	GLFWMouseManager::GetMouseManager()->processMouse((int32_t)x, (int32_t)y);
}
