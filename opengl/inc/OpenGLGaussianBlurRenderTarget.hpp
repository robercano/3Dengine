/**
 * @class	OpenGLGaussianBlurRenderTarget
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include "GaussianBlurRenderTarget.hpp"
#include "OpenGL.h"
#include "OpenGLFilterRenderTarget.hpp"
#include "Shader.hpp"

#pragma warning(disable : 4250)

class OpenGLGaussianBlurRenderTarget : public GaussianBlurRenderTarget, public OpenGLFilterRenderTarget
{
    public:
        /**
         * Constructor
         *
         * Uses HDR framebuffer
         */
        OpenGLGaussianBlurRenderTarget() : OpenGLFilterRenderTarget(true) {}

    private:
        bool customInit()
        {
            std::string error;
            if (_shader->use("filters/gaussian_blur", error) == false) {
                printf("ERROR loading shader filters/gaussian_blur: %s\n", error.c_str());
                return false;
            }
            return true;
        }
        void setCustomParams(void)
        {
            _shader->setUniformBool("u_horizontal", _horizontal);
            if (_horizontal == true) {
                _shader->setUniformFloat("u_texelSize", 1.0f / _width);
            } else {
                _shader->setUniformFloat("u_texelSize", 1.0f / _height);
            }
        }
        void unsetCustomParams(void) { }
};
