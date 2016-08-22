/**
 * @class	OpenGLHDRRenderTarget
 * @brief	Render target for OpenGL. A render target allows to render objects in HDR
 *          with a color buffer of 16-bits floating point. When the render target is resolved
 *          then tone mapping is applied
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include "HDRRenderTarget.hpp"
#include "OpenGLFilterRenderTarget.hpp"
#include "Shader.hpp"

#pragma warning(disable : 4250)

class OpenGLHDRRenderTarget : public HDRRenderTarget, public OpenGLFilterRenderTarget
{
    public:
        OpenGLHDRRenderTarget() : OpenGLFilterRenderTarget(true) {}

    private:
        bool customInit()
        {
            std::string error;
            if (_shader->use("hdr/hdr", error) == false) {
                printf("ERROR loading shader hdr/hdr: %s\n", error.c_str());
                return false;
            }
            return true;
        }
        void setCustomParams(void)
        {
            _shader->setUniformFloat("u_exposure", _exposure);
            _shader->setUniformBool("u_tonemapping", _toneMapping);
        }
        void unsetCustomParams(void) { }
};
