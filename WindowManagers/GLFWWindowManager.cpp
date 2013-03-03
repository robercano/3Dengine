/*
 * @class	GLFWWindowManager
 * @brief	GLFW based window manager
 *
 * @author	Roberto Sosa Cano
 * @todo	Standarize the interface to make a generic Window Manager
 */

#include "GLFWWindowManager.hpp"
#include "GLFWKeyManager.hpp"
#include "GLFWMouseManager.hpp"

GLFWWindowManager::GLFWWindowManager() : _width(0), _height(0), _renderer(NULL)
{
	_alive = true;
}

GLFWWindowManager::~GLFWWindowManager()
{
	stop();
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
	glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4);
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
	return true;
}

bool GLFWWindowManager::setRenderer(Renderer *renderer)
{
	_renderer = renderer;
	return true;
}

void GLFWWindowManager::loop(void)
{
	double start = glfwGetTime();
	uint32_t frames = 0;
	while (_alive)
	{
		_renderer->render();
		glfwSwapBuffers();
		if ((++frames)%100 == 0) {
			printf("Framerate %.1f fps\n", frames/(glfwGetTime()-start));
		}
	}
}

void GLFWWindowManager::stop(void)
{
	_alive = false;
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
