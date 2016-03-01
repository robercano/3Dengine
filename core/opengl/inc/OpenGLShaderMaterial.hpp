/**
 * @class	OpenGLShaderMaterial
 * @brief	Class to abstract a shader material. A material
 *          contains ambient, diffuse, specular, alpha and shininess
 *          components
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include <glm/glm.hpp>
#include "OpenGL.h"

class OpenGLShaderMaterial
{
	public:
		OpenGLShaderMaterial() : _uniformBO(0) {}
		~OpenGLShaderMaterial();
        bool bindToProgram(GLuint programID);
    private:
        GLuint _uniformBO;
};
