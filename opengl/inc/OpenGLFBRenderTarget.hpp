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

#include "FBRenderTarget.hpp"
#include "OpenGL.h"
#include "Shader.hpp"

class OpenGLFBRenderTarget : public FBRenderTarget
{
  public:
    ~OpenGLFBRenderTarget();
    bool init(uint32_t width, uint32_t height, uint32_t maxSamples, uint32_t numTargets);
    void bind();
    void bindDepth();
    void unbind();
    bool blit(uint32_t dstX, uint32_t dstY, uint32_t width, uint32_t height, uint32_t target = 0, bool bindMainFB = true);
    void clear();

  private:
    GLuint _frameBuffer;  /**< Frame buffer object containing the color and depth buffers */
    uint32_t _numTargets; /**< Number of color attachments for this target */
    GLuint *_colorBuffer; /**< Array of GL allocated IDs for the color buffers */
    GLuint *_attachments; /**< Array of color attachments locations for the draw buffers */
    GLuint _depthBuffer;  /**< GL allocated ID for the depth buffer */
};
