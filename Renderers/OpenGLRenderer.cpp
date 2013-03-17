/**
 * @class	OpenGLRenderer
 * @brief	OpenGL based 3D renderer
 *
 * @author	Roberto Sosa Cano
 */

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include "OpenGLRenderer.hpp"
#include "OpenGLShader.hpp"

OpenGLRenderer::~OpenGLRenderer()
{
	std::vector<Object3D*>::iterator it = _objects.begin();
	for (; it != _objects.end(); ++it) {
		(*it)->destroy();
	}
}

void OpenGLRenderer::init()
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

Shader * OpenGLRenderer::getShader(void)
{
	return new OpenGLShader();
}

bool OpenGLRenderer::render(const glm::mat4 &projection, const glm::mat4 &view)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	std::vector<Object3D*>::iterator it = _objects.begin();
	for (; it != _objects.end(); ++it) {
		(*it)->render(projection, view);		// TODO: Not sure this should be like this
	}
}

bool OpenGLRenderer::resize(uint16_t width, uint16_t height)
{
	_width  = width;
	_height = height;
}

bool OpenGLRenderer::addObject(Object3D *object)
{
	object->init();
	_objects.push_back(object);
}
