/**
 * @class	OpenGLFXAARenderTarget
 * @brief	Render target for OpenGL. A render target allows to render objects to it
 *          instead of to the main screen. Then the target can be rendered to the main screen as
 *          a texture
 *
 *          The FXAA render target applies Fast Approximate Anti-Aliasing from
 *          Timothy Lottes paper t Nvidia
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include "FXAARenderTarget.hpp"
#include "OpenGL.h"
#include "OpenGLFilterRenderTarget.hpp"
#include "Shader.hpp"

#pragma warning(disable : 4250)

class OpenGLFXAARenderTarget : public FXAARenderTarget, public OpenGLFilterRenderTarget
{
  private:
    bool customInit()
    {
        std::string error;
        if (_shader->use("anti-aliasing/fxaa_lottes", error) == false) {
            printf("ERROR loading shader anti-aliasing/fxaa_lottes: %s\n", error.c_str());
            return false;
        }
        return true;
    }
    void setCustomParams(void)
    {
        glm::vec2 rpcFrame(1.0f / _width, 1.0f / _height);
        _shader->setUniformVec2("f_rpcFrame", rpcFrame);
        __(glDisable(GL_BLEND));
        __(glDisable(GL_DEPTH_TEST));
    }
    void unsetCustomParams(void) { glEnable(GL_DEPTH_TEST); }
};
