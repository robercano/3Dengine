/**
 * @class	OpenGLHDRRenderTarget
 * @brief	Render target for OpenGL. A render target allows to render objects in HDR
 *          with a color buffer of 16-bits floating point. When the render target is resolved
 *          then tone mapping is applied
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include "OpenGL.h"
#include "HDRRenderTarget.hpp"
#include "Shader.hpp"

#pragma warning(disable : 4250)

class OpenGLHDRRenderTarget : public HDRRenderTarget
{
  public:
    ~OpenGLHDRRenderTarget();
    bool init(uint32_t width, uint32_t height, uint32_t maxSamples = 0);
    void bind();
    void bindDepth();
    void unbind();
    bool blit(uint32_t dstX, uint32_t dstY, uint32_t width, uint32_t height, bool bindMainFB = true);
    void clear();

  protected:
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
