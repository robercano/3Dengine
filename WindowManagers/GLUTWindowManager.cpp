/*
 * @class	GLUTManager
 * @brief	GLUT based window manager
 *
 * @author	Roberto Sosa Cano
 * @todo	Standarize the interface to make a generic Window Manager
 */

#include "GLUTWindowManager.hpp"
#include <GL/freeglut.h>

/* Internal reference to the object for the C like callbacks */
static GLUTWindowManager *_thiz = NULL;

GLUTWindowManager::GLUTWindowManager() : _width(0), _height(0), _renderer(NULL)
{
	_thiz = this;
}

KeyManager *GLUTWindowManager::getKeyManager()
{
	return NULL;
}
MouseManager *GLUTWindowManager::getMouseManager()
{
	return NULL;
}

bool GLUTWindowManager::init()
{
	char *argv;
	int argc = 0;
	glutInit(&argc, &argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);

	/* Ensure OpenGL 3.0 compatibility */
	//glutInitContextVersion(3, 0);
	//glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	//glutInitContextProfile(GLUT_CORE_PROFILE);

	return true;
}

bool GLUTWindowManager::createWindow(std::string &name, uint16_t width, uint16_t height, bool fullscreen)
{
	_width  = width;
	_height = height;

	glutInitWindowSize(_width, _height);
	glutCreateWindow(name.c_str());
	glutSetWindowTitle((const char *)glGetString(GL_VERSION));
	return true;
}

bool GLUTWindowManager::setRenderer(Renderer *renderer)
{
	glutDisplayFunc(GLUTWindowManager::display_callback);
	glutReshapeFunc(GLUTWindowManager::resize_callback);

	_renderer = renderer;
	return true;
}

void GLUTWindowManager::swapBuffers(void)
{
	glutSwapBuffers();
}

void GLUTWindowManager::display_callback(void)
{
	_thiz->display();
}

void GLUTWindowManager::display(void)
{
	if (_renderer) {
		_renderer->render();
	}
	glutSwapBuffers();
}

void GLUTWindowManager::resize_callback(int width, int height)
{
	WindowManager *windowManager = WindowManager::GetCurrentManager();

	if (windowManager) {
		windowManager->resize((uint16_t)width, (uint16_t)height);
	}
}

bool GLUTWindowManager::resize(uint16_t width, uint16_t height)
{
	if (_renderer) {
		_renderer->resize(width, height);
	}
}

void GLUTWindowManager::poll(void)
{
}
