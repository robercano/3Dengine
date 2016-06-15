/**
 * @class	OpenGLToonLightingShader
 *
 * @brief   This class re-uses the Blinn-Phong shader but it enables
 *          the toon lighting by setting the corresponding flag to 1. This
 *          is done this way so we can re-use most of the Blinn-Phong shader
 *          code
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include "OpenGL.h"
#include "OpenGLLightingShader.hpp"
#include "OpenGLShader.hpp"
#include "Shader.hpp"
#include "ToonLightingShader.hpp"

#pragma warning(disable : 4250)

class OpenGLToonLightingShader : public virtual ToonLightingShader, public OpenGLLightingShader
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

	void setCustomParams()
	{
		setUniformUint("u_enableToon", 1);
	}
};
