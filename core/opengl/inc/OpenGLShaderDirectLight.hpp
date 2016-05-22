/**
 * @class	OpenGLShaderDirectLight
 * @brief	OpenGL direct light implemented as a block uniform to be used in
 *          a shader
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include "OpenGL.h"
#include "OpenGLUniformBlock.hpp"
#include "DirectLight.hpp"

class OpenGLShaderDirectLight : public OpenGLUniformBlock
{
	public:
        void init(uint32_t bindingPoint);
        void copyLight(DirectLight &light);
};
