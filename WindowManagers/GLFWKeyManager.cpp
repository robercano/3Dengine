/**
 * @class	GLFWKeyManager
 * @brief	GLFW based key manager
 *
 * @author	Roberto Sosa Cano
 */
#include <GL/glfw.h>
#include "GLFWKeyManager.hpp"

GLFWKeyManager *GLFWKeyManager::_keyManager = NULL;

GLFWKeyManager *GLFWKeyManager::GetKeyManager(void)
{
	if (_keyManager == NULL) {
		_keyManager = new GLFWKeyManager();
	}
	return _keyManager;
}

void GLFWKeyManager::DisposeKeyManager(void)
{
	delete _keyManager;
}

GLFWKeyManager::GLFWKeyManager(void)
{
	glfwSetKeyCallback(keyCallback);
	glfwEnable(GLFW_KEY_REPEAT);
}

GLFWKeyManager::~GLFWKeyManager(void)
{
	glfwSetKeyCallback(NULL);
}

bool GLFWKeyManager::registerListener(KeyListener &listener, std::vector<uint32_t> &keys)
{
	std::vector<uint32_t>::iterator it;
	for (it = keys.begin(); it != keys.end(); ++it) {
		_listeners[*it].push_back(&listener);
	}
    return true;
}

void GLFWKeyManager::processKey(uint32_t key, bool state)
{
	std::map<uint32_t, std::vector<KeyListener*> >::iterator it = _listeners.find(key);
	if (it == _listeners.end()) {
		return;
	}
	std::vector<KeyListener *>::iterator listener;
	for (listener = it->second.begin(); listener != it->second.end(); ++listener) {
		(*listener)->processKey(key, state);
	}
}

void GLFWKeyManager::keyCallback(int key, int state)
{
	GLFWKeyManager::GetKeyManager()->processKey(key, state == GLFW_PRESS);
}
