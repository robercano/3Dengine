/**
 * @class	OpenGLShaderLight
 * @brief	OpenGL material implemented as a block uniform to be used in
 *          a shader. Contains ambient, diffuse, specular, alpha and shininess
 *          components
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include "OpenGL.h"
#include "ShaderLight.hpp"
#include "OpenGLUniformBlock.hpp"

class OpenGLShaderLight : public OpenGLUniformBlock, public ShaderLight
{
	public:
        OpenGLShaderLight(uint32_t bindingPoint);
        bool copyLight(Light &material);
};
