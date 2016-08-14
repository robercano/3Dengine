/**
 * @class	OpenGLFilterRenderTarget
 * @brief	Render target for OpenGL. A render target allows to render objects to it
 *          instead of to the main screen. Then the target can be rendered to the main screen as
 *          a texture
 *
 *          The Filter render target applies no anti-aliasing
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include "OpenGL.h"
#include "RenderTarget.hpp"
#include "Shader.hpp"

#pragma warning(disable : 4250)

class OpenGLFilterRenderTarget : public virtual RenderTarget
{
  public:
      OpenGLFilterRenderTarget(bool useHDR = false) : _useHDR(useHDR) {}
    ~OpenGLFilterRenderTarget();
    bool init(uint32_t width, uint32_t height, uint32_t maxSamples = 0, uint32_t numTargets = 1);
    void bind();
    void bindDepth();
    void unbind();
    bool blit(uint32_t dstX, uint32_t dstY, uint32_t width, uint32_t height, uint32_t target = 0, bool bindMainFB = true);
    void clear();

  protected:
    virtual bool customInit(void) = 0;
    virtual void setCustomParams(void) = 0;
    virtual void unsetCustomParams(void) = 0;

    bool _useHDR;         /**< Indicates this filter render target must use an HDR buffer. In that case
                               the associated shader is responsible for the tone mapping */
    GLuint _frameBuffer;  /**< Frame buffer object containing the color and depth buffers */
    uint32_t _numTargets; /**< Number of color attachments for this target */
    GLuint *_colorBuffer; /**< Array of GL allocated IDs for the color buffers */
    GLuint *_attachments; /**< Array of color attachments locations for the draw buffers */
    GLuint _depthBuffer;  /**< GL allocated ID for the depth buffer */
    GLuint _vertexArray;  /**< VAO for the render target surface */
    GLuint _vertexBuffer; /**< VBO for the render target surface */

    Shader *_shader;      /**< Shader used to render the target onto another target or
                               framebuffer */
};
