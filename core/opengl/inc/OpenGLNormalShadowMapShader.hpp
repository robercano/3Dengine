/**
 * @class	OpenGLNormalShadowMapShader
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include "OpenGL.h"
#include "Shader.hpp"
#include "NormalShadowMapShader.hpp"
#include "OpenGLShader.hpp"
#include "OpenGLLightingShader.hpp"

#pragma warning( disable : 4250 )

class OpenGLNormalShadowMapShader : public NormalShadowMapShader, public OpenGLShader
{
	public:
		bool init()
		{
			std::string error;

			if (use("shadows/shadowmap_normal", error) != true) {
				printf("ERROR loading shader shadows/shadowmap_normal: %s\n", error.c_str());
				return false;
			}
            return true;
		}
};
