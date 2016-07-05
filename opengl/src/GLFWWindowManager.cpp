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
#include "Logging.hpp"

using namespace Logging;

GLFWWindowManager::GLFWWindowManager() : _width(0), _height(0), _renderer(NULL) {}
GLFWWindowManager::~GLFWWindowManager()
{
    GLFWMouseManager::DisposeMouseManager();
    GLFWKeyManager::DisposeKeyManager();
    glfwDestroyWindow(_window);
    glfwTerminate();
}

KeyManager *GLFWWindowManager::getKeyManager() { return GLFWKeyManager::GetKeyManager(); }
MouseManager *GLFWWindowManager::getMouseManager() { return GLFWMouseManager::GetMouseManager(); }
bool GLFWWindowManager::init()
{
    if (!glfwInit()) {
        log("Failed to initialize GLFW\n");
        return false;
    }
    return true;
}

bool GLFWWindowManager::createWindow(std::string &name, uint16_t width, uint16_t height, bool fullscreen)
{
    _width = width;
    _height = height;
    _fullscreen = fullscreen;

    /* Detect the current resolution of the primary monitor */
    GLFWmonitor *primary = glfwGetPrimaryMonitor();
    if (primary == NULL) {
        log("Failed to detect primary monitor\n");
        return false;
    }

    const GLFWvidmode *videoMode = glfwGetVideoMode(primary);
    if (videoMode == NULL) {
        log("Failed to detect primary monitor video mode\n");
        return false;
    }

    if (width == 0 || height == 0) {
        width = videoMode->width;
        height = videoMode->height;
    }

    /* Request OpenGL 3.2 */
    // glfwWindowHint(GLFW_FSAA_SAMPLES, 4);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_SRGB_CAPABLE, GL_FALSE);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);

    _window = glfwCreateWindow(_width, _height, name.c_str(), fullscreen ? glfwGetPrimaryMonitor() : NULL, NULL);
    glfwMakeContextCurrent(_window);  // Initialize GLEW

    /* Initialize GLEW */
    glewExperimental = true;  // Needed for core profile
    if (glewInit() != GLEW_OK) {
        log("Failed to initialize GLEW\n");
        return false;
    }

    glfwSetWindowSizeCallback(_window, handle_resize);
    glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
    glfwGetWindowSize(_window, &w, &h);
    *width = w;
    *height = h;
    return true;
}

void GLFWWindowManager::swapBuffers(void) { glfwSwapBuffers(_window); }
void GLFWWindowManager::handle_resize(GLFWwindow *w, int width, int height) { WindowManager::GetInstance()->resize(width, height); }
bool GLFWWindowManager::resize(uint16_t width, uint16_t height)
{
    if (_renderer) {
        _renderer->resize(width, height);
    }
    return true;
}

void GLFWWindowManager::poll(void) { glfwPollEvents(); }
