/**
 * @class	OpenGLRenderer
 * @brief	OpenGL based 3D renderer
 *
 * @author	Roberto Sosa Cano
 */

#include "OpenGLRenderer.hpp"
#include <OpenGL/gl.h>

void OpenGLRenderer::init()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
}

bool OpenGLRenderer::render()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	std::vector<Object3D*>::iterator it = _objects.begin();
	for (; it != _objects.end(); ++it) {
		(*it)->render();
	}

	glFlush();
}

bool OpenGLRenderer::resize(uint16_t width, uint16_t height)
{
	_width  = width;
	_height = height;

	glViewport(0,0,width,height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//TODO: gluOrtho2D(0,width,0,height);
	glMatrixMode(GL_MODELVIEW);
}

bool OpenGLRenderer::addObject(Object3D *object)
{
	_objects.push_back(object);
}
