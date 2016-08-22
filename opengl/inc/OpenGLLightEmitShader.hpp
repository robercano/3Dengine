/**
 * @class	OpenGLLightEmitShader
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include "LightEmitShader.hpp"
#include "OpenGL.h"
#include "OpenGLLightingShader.hpp"
#include "OpenGLShader.hpp"
#include "Shader.hpp"

#pragma warning(disable : 4250)

class OpenGLLightEmitShader : public virtual LightEmitShader, public OpenGLLightingShader
{
  public:
    bool init()
    {
        std::string error;

        if (use("lighting/lightemit", error) != true) {
            printf("ERROR loading shader lighting/lightemit: %s\n", error.c_str());
            return false;
        }

        return OpenGLLightingShader::init();
    }

    void setCustomParams() {}
};
