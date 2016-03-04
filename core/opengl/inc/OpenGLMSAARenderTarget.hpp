/**
 * @class	OpenGLNOAARenderTarget
 * @brief	Render target for OpenGL. A render target allows to render objects to it
 *          instead of to the main screen. Then the target can be rendered to the main screen as
 *          a texture
 *
 *          The NOAA render target applies no anti-aliasing
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include "OpenGL.h"
#include "Shader.hpp"
#include "MSAARenderTarget.hpp"

class OpenGLMSAARenderTarget : public MSAARenderTarget
{
	public:
        ~OpenGLMSAARenderTarget();
        bool init(uint32_t width, uint32_t height, uint32_t samples);
        void bind();
        void unbind();
        bool blit(uint32_t dstX, uint32_t dstY, uint32_t width, uint32_t height);
        void clear();
        void clear(float r, float g, float b, float a);

        static uint32_t getMaxSamples();

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

        uint32_t _samples;
};
