/**
 * @class	OpenGLSSAARenderTarget
 * @brief	Render target for OpenGL. A render target allows to render objects to it
 *          instead of to the main screen. Then the target can be rendered to the main screen as
 *          a texture
 *
 *          The SSAA render target implements super sampled anti-aliasing
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include "OpenGL.h"
#include "SSAARenderTarget.hpp"
#include "Shader.hpp"

class OpenGLSSAARenderTarget : public SSAARenderTarget
{
  public:
    ~OpenGLSSAARenderTarget();
    bool init(uint32_t width, uint32_t height, uint32_t factor, uint32_t numTargets = 1);
    void bind();
    void bindDepth();
    void unbind();
    bool blit(uint32_t dstX, uint32_t dstY, uint32_t width, uint32_t height, uint32_t target = 0, bool bindMainFB = true);
    bool blit() { return blit(0, 0, _width/_factor, _height/_factor); }
    void clear();

  private:

    GLuint _frameBuffer;  /**< Frame buffer object containing the color and depth buffers */
    uint32_t _numTargets; /**< Number of color attachments for this target */
    GLuint *_colorBuffer; /**< Array of GL allocated IDs for the color buffers */
    GLuint *_attachments; /**< Array of color attachments locations for the draw buffers */
    GLuint _depthBuffer;  /**< GL allocated ID for the depth buffer */
    GLuint _vertexArray;  /**< VAO for the render target surface */
    GLuint _vertexBuffer; /**< VBO for the render target surface */

    uint32_t _factor;     /**< Factor for the internal size of the SSAA framebuffer */
};
