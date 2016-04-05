/**
 * @class	OpenGLFBRenderTarget
 * @brief	Render target for OpenGL. A render target allows to render objects to it
 *          instead of to the main screen. Then the target can be rendered to the main screen as
 *          a texture
 *
 *          The FB render target uses no effect-shader and performs no blending, it
 *          just does a copy-pixel operation
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include "OpenGL.h"
#include "Shader.hpp"
#include "FBRenderTarget.hpp"

class OpenGLFBRenderTarget : public FBRenderTarget
{
	public:
        ~OpenGLFBRenderTarget();
        bool init(uint32_t width, uint32_t height, uint32_t maxSamples);
        void bind();
        void unbind();
        bool blit(uint32_t dstX, uint32_t dstY, uint32_t width, uint32_t height, bool bindMainFB = true);
        void clear();

    private:
        /**
         * Frame buffer object ID to reference
         * both the color buffer and the depth buffer
         */
        GLuint _frameBuffer;

        /**
         * Frame buffer texture to hold the color buffer
         */
        GLuint _colorBuffer;

        /**
         * Render buffer object to hold the depth buffer
         */
        GLuint _depthBuffer;
};
