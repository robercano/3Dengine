/**
 * @class	OpenGLToonLightingShader
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

        if (use("lighting/toon", error) != true) {
            printf("ERROR loading shader lighting/toon: %s\n", error.c_str());
            return false;
        }

        return OpenGLLightingShader::init();
    }
};
