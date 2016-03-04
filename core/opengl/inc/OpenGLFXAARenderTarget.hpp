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

#include "OpenGL.h"
#include "Shader.hpp"
#include "FXAARenderTarget.hpp"

class OpenGLFXAARenderTarget : public FXAARenderTarget
{
	public:
        ~OpenGLFXAARenderTarget();
        bool init(uint32_t width, uint32_t height);
        void bind();
        void unbind();
        bool blit(uint32_t dstX, uint32_t dstY, uint32_t width, uint32_t height);
        void clear(float r, float g, float b, float a);

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

        /**
         * Render target vertices buffer
         */
        GLuint _vertexArray;
        GLuint _vertexBuffer;

        /**
         * Shader for the target rendering to screen
         */
        Shader *_shader;
};
