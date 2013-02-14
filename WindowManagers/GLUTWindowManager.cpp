/*
 * @class	GLUTManager
 * @brief	GLUT based window manager
 *
 * @author	Roberto Sosa Cano
 * @todo	Standarize the interface to make a generic Window Manager
 */

#include "GLUTWindowManager.hpp"
#include <GLUT/glut.h>

/* Internal reference to the object for the C like callbacks */
static GLUTWindowManager *_thiz = NULL;

GLUTWindowManager::GLUTWindowManager() : _width(0), _height(0), _renderer(NULL)
{
	_thiz = this;
}

bool GLUTWindowManager::init()
{
	char *argv;
	int argc = 0;
	glutInit(&argc, &argv);
	return true;
}

bool GLUTWindowManager::createWindow(std::string &name, uint16_t width, uint16_t height)
{
	_width  = width;
	_height = height;

	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(_width, _height);
	glutCreateWindow(name.c_str());
	return true;
}

bool GLUTWindowManager::setRenderer(Renderer *renderer)
{
	glutDisplayFunc(GLUTWindowManager::display_callback);
	glutReshapeFunc(GLUTWindowManager::resize_callback);

	_renderer = renderer;
	return true;
}

void GLUTWindowManager::loop(void)
{
	glutMainLoop();
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
	_thiz->resize((uint16_t)width, (uint16_t)height);
}

void GLUTWindowManager::resize(uint16_t width, uint16_t height)
{
	if (_renderer) {
		_renderer->resize(width, height);
	}
}

