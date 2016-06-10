/**
 * @class	OpenGLBlinnPhongShader
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include "OpenGL.h"
#include "Shader.hpp"
#include "BlinnPhongShader.hpp"
#include "OpenGLShader.hpp"
#include "OpenGLLightingShader.hpp"

#pragma warning( disable : 4250 )

class OpenGLBlinnPhongShader : public virtual BlinnPhongShader, public OpenGLLightingShader
{
	public:
		bool init()
		{
			std::string error;

			if (use("lighting/blinnphong_reflection", error) != true) {
				printf("ERROR loading shader lighting/blinnphong_reflection: %s\n", error.c_str());
				return false;
			}

			return OpenGLLightingShader::init();
		}
};
