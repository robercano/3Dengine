/**
 * @class	OpenGLShaderSpotLight
 * @brief	OpenGL spot light implemented as a block uniform to be used in
 *          a shader
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include "OpenGL.h"
#include "OpenGLUniformBlock.hpp"
#include "SpotLight.hpp"

class OpenGLShaderSpotLight : public OpenGLUniformBlock
{
	public:
        void init(uint32_t bindingPoint, uint32_t lightIndex);
        void copyLight(SpotLight &light);
};
