/**
 * @class	OpenGLSolidColoringShader
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include "OpenGL.h"
#include "OpenGLLightingShader.hpp"
#include "OpenGLShader.hpp"
#include "Shader.hpp"
#include "SolidColoringShader.hpp"

#pragma warning(disable : 4250)

class OpenGLSolidColoringShader : public virtual SolidColoringShader, public OpenGLLightingShader
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

    void setCustomParams() { setUniformUint("u_enableToon", 1); }
};
