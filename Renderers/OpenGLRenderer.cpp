/**
 * @class	OpenGLRenderer
 * @brief	OpenGL based 3D renderer
 *
 * @author	Roberto Sosa Cano
 */

#include <GL/glew.h>
#include <GL/glfw.h>
#ifdef __linux
#include <GL/gl.h>
#include <GL/glu.h>
#else
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#endif
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
	//glClearColor(1.0, 1.0, 1.0, 1.0);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

    glDisable(GL_DITHER);
    glDisable(GL_POINT_SMOOTH);
    glDisable(GL_LINE_SMOOTH);
    glDisable(GL_POLYGON_SMOOTH);
    glHint(GL_POINT_SMOOTH, GL_DONT_CARE);
    glHint(GL_LINE_SMOOTH, GL_DONT_CARE);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_DONT_CARE);
#define GL_MULTISAMPLE_ARB 0x809D
    glDisable( GL_MULTISAMPLE_ARB) ;

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

Shader * OpenGLRenderer::getShader(void)
{
	return new OpenGLShader();
}

bool OpenGLRenderer::render(const glm::mat4 &projection, const glm::mat4 &view, RenderTarget *renderTarget)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	std::vector<Object3D*>::iterator it = _objects.begin();
	for (; it != _objects.end(); ++it) {
		(*it)->render(projection, view, renderTarget);		// TODO: Not sure this should be like this
	}

    if (renderTarget) {
        renderTarget->render();
    }

    return true;
}

bool OpenGLRenderer::resize(uint16_t width, uint16_t height)
{
	_width  = width;
	_height = height;
    return true;
}

bool OpenGLRenderer::addObject(Object3D *object)
{
	object->init();
	_objects.push_back(object);
    return true;
}
