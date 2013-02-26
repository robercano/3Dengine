/*
 * @class	GLFWWindowManager
 * @brief	GLFW based window manager
 *
 * @author	Roberto Sosa Cano
 * @todo	Standarize the interface to make a generic Window Manager
 */

#include "GLFWWindowManager.hpp"

GLFWWindowManager::GLFWWindowManager() : _width(0), _height(0), _renderer(NULL)
{
}

bool GLFWWindowManager::init()
{
	glfwInit();
	return true;
}

bool GLFWWindowManager::createWindow(std::string &name, uint16_t width, uint16_t height)
{
	_width  = width;
	_height = height;

	/* Request OpenGL 3.2 */
	glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 2);
	glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwOpenWindow(_width, _height, 0, 0, 0, 0, 32, 0, GLFW_WINDOW);

	/* Initialize GLEW */
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return false;
	}

	glfwSetWindowTitle(name.c_str());
    glfwSetWindowSizeCallback(handle_resize);
	return true;
}

bool GLFWWindowManager::setRenderer(Renderer *renderer)
{
	_renderer = renderer;
	return true;
}

void GLFWWindowManager::loop(void)
{
	while (true)
	{
		_renderer->render();
		glfwSwapBuffers();
	}
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
}
