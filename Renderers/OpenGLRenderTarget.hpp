/**
 * @class	OpenGLRenderTarget
 * @brief	Render target for OpenGL. A render target allows to render objects to it
 *          instead of to the main screen. Then the target can be rendered to the main screen as
 *          a texture
 *
 * @author	Roberto Sosa Cano (http://www.robertocano.es)
 */
#pragma once

#include "OpenGL.h"
#include "Shader.hpp"
#include "RenderTarget.hpp"

class OpenGLRenderTarget : public RenderTarget
{
	public:
        /**
         * Initializes the target with the given size
         *
         * @param width  Width of the render target
         * @param height Height of the render target
         */
        bool init(uint32_t width, uint32_t height);

       /**
        * Returns the render target ID that will be used by the renderer
        * to render the objects into the target
        *
        */
        const uint32_t getID();

        /**
         * Renders the target to screen
         */
        bool render();

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
