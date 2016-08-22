/**
 * @class	OpenGLFlatShader
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include "FlatShader.hpp"
#include "OpenGL.h"
#include "OpenGLLightingShader.hpp"
#include "OpenGLShader.hpp"
#include "Shader.hpp"

#pragma warning(disable : 4250)

class OpenGLFlatShader : public virtual FlatShader, public OpenGLLightingShader
{
  public:
    bool init()
    {
        std::string error;

        if (use("lighting/flat", error) != true) {
            printf("ERROR loading shader lighting/flat: %s\n", error.c_str());
            return false;
        }

        return OpenGLLightingShader::init();
    }

    void setCustomParams() {}
};
