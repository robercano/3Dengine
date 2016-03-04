/**
 * @class	GLFWKeyManager
 * @brief	GLFW based key manager
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
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
	glfwSetKeyCallback(glfwGetCurrentContext(), keyCallback);
}

GLFWKeyManager::~GLFWKeyManager(void)
{
	glfwSetKeyCallback(glfwGetCurrentContext(), NULL);
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

void GLFWKeyManager::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	GLFWKeyManager::GetKeyManager()->processKey(key, action == GLFW_PRESS || action == GLFW_REPEAT);
}
