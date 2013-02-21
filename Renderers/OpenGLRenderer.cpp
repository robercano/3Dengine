/**
 * @class	OpenGLRenderer
 * @brief	OpenGL based 3D renderer
 *
 * @author	Roberto Sosa Cano
 */

#include "OpenGLRenderer.hpp"
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>

void OpenGLRenderer::init()
{
/*	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);*/
}

bool OpenGLRenderer::render()
{
	std::vector<Object3D*>::iterator it = _objects.begin();
	for (; it != _objects.end(); ++it) {
		(*it)->render();
	}
}

bool OpenGLRenderer::resize(uint16_t width, uint16_t height)
{
	printf("Resizzing OPengl!! %d, %d\n", width, height);
	_width  = width;
	_height = height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

	//TODO: gluOrtho2D(0,width,0,height);
    gluPerspective( 45.0f,                      //camera angle
                (GLfloat)width/(GLfloat)height, //The width to height ratio
                 1.0f,                          //The near z clipping coordinate
                100.0f );                       //The far z clipping coordinate
}

bool OpenGLRenderer::addObject(Object3D *object)
{
	_objects.push_back(object);
}
