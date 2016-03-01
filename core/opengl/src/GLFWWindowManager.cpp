/*
 * @class	GLFWWindowManager
 * @brief	GLFW based window manager
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 * @todo	Standarize the interface to make a generic Window Manager
 */

#include "GLFWWindowManager.hpp"
#include "GLFWKeyManager.hpp"
#include "GLFWMouseManager.hpp"

GLFWWindowManager::GLFWWindowManager() : _width(0), _height(0), _renderer(NULL)
{
}

GLFWWindowManager::~GLFWWindowManager()
{
	GLFWMouseManager::DisposeMouseManager();
	GLFWKeyManager::DisposeKeyManager();
	glfwTerminate();
}

KeyManager *GLFWWindowManager::getKeyManager()
{
	return GLFWKeyManager::GetKeyManager();
}

MouseManager *GLFWWindowManager::getMouseManager()
{
	return GLFWMouseManager::GetMouseManager();
}

bool GLFWWindowManager::init()
{
	glfwInit();
	return true;
}

bool GLFWWindowManager::createWindow(std::string &name, uint16_t width, uint16_t height, bool fullscreen)
{
	_width  = width;
	_height = height;

	/* Request OpenGL 3.2 */
	//glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 2);
	glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwOpenWindow(_width, _height, 8, 8, 8, 8, 32, 8, fullscreen ? GLFW_FULLSCREEN : GLFW_WINDOW);

	/* Initialize GLEW */
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return false;
	}

	glfwSetWindowTitle(name.c_str());
    glfwSetWindowSizeCallback(handle_resize);
	glfwDisable(GLFW_MOUSE_CURSOR);

	/* Poll for events explicitly */
	glfwDisable(GLFW_AUTO_POLL_EVENTS);
	return true;
}

bool GLFWWindowManager::setRenderer(Renderer *renderer)
{
	_renderer = renderer;
	return true;
}

bool GLFWWindowManager::getWindowSize(uint32_t *width, uint32_t *height)
{
    int w, h;
    glfwGetWindowSize(&w, &h);
    *width = w;
    *height = h;
    return true;
}

void GLFWWindowManager::swapBuffers(void)
{
	glfwSwapBuffers();
}

void GLFWCALL GLFWWindowManager::handle_resize(int width,int height)
{
	WindowManager *windowManager = WindowManager::GetCurrentManager();

	if (windowManager) {
		windowManager->resize(width, height);
	}
}

bool GLFWWindowManager::resize(uint16_t width, uint16_t height)
{
	if (_renderer) {
		_renderer->resize(width, height);
	}
    return true;
}

void GLFWWindowManager::poll(void)
{
	glfwPollEvents();
}
