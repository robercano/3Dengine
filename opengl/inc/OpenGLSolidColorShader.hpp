/**
 * @class	OpenGLSolidColorShader
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include "OpenGL.h"
#include "OpenGLLightingShader.hpp"
#include "OpenGLShader.hpp"
#include "Shader.hpp"
#include "SolidColorShader.hpp"
#include "Logging.hpp"

#pragma warning(disable : 4250)

class OpenGLSolidColorShader : public virtual SolidColorShader, public OpenGLShader
{
  public:
    bool init()
    {
        std::string error;

        if (use("utils/render_solidcolor", error) != true) {
            Logging::log("ERROR loading shader utils/render_solidcolor: %s\n", error.c_str());
            return false;
        }

        return true;
    }

    void setCustomParams() { setUniformVec4("u_color", _color); }
};
