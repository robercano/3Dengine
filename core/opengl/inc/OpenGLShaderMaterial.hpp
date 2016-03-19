/**
 * @class	OpenGLShaderMaterial
 * @brief	OpenGL material implemented as a block uniform to be used in
 *          a shader. Contains ambient, diffuse, specular, alpha and shininess
 *          components
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include "OpenGL.h"
#include "ShaderMaterial.hpp"
#include "OpenGLUniformBlock.hpp"

class OpenGLShaderMaterial : public OpenGLUniformBlock, public ShaderMaterial
{
	public:
        OpenGLShaderMaterial(uint32_t bindingPoint);
        bool copyMaterial(Material &material);
};
