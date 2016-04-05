/**
 * @class	OpenGLFBRenderTarget
 * @brief	Render target for OpenGL. A render target allows to render objects to it
 *          instead of to the main screen. Then the target can be rendered to the main screen as
 *          a texture
 *
 *          The FB render target applies no anti-alising to the rendering
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#include <glm/gtc/matrix_transform.hpp>
#include "OpenGL.h"
#include "OpenGLFBRenderTarget.hpp"
#include "Renderer.hpp"
#include "WindowManager.hpp"

OpenGLFBRenderTarget::~OpenGLFBRenderTarget()
{
    GL( glDeleteTextures(1, &_colorBuffer) );
    GL( glDeleteRenderbuffers(1, &_depthBuffer) );
    GL( glDeleteFramebuffers(1, &_frameBuffer) );
}

bool OpenGLFBRenderTarget::init(uint32_t width, uint32_t height, uint32_t maxSamples)
{
	(void)maxSamples;

    /* Texture buffer */
    GL( glGenTextures(1, &_colorBuffer) );
    GL( glBindTexture(GL_TEXTURE_2D, _colorBuffer) );
    {
        GL( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR) );
        GL( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR) );
        GL( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE) );
        GL( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE) );
        GL( glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL) );
    }
    GL( glBindTexture(GL_TEXTURE_2D, 0) );

    /* Depth buffer */
    GL( glGenRenderbuffers(1, &_depthBuffer) );
    GL( glBindRenderbuffer(GL_RENDERBUFFER, _depthBuffer) );
    {
        GL( glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, width, height) );
    }
    GL( glBindRenderbuffer(GL_RENDERBUFFER, 0) );

    /* Framebuffer to link everything together */
    GL( glGenFramebuffers(1, &_frameBuffer) );
    GL( glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer) );
    {
        GL( glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _colorBuffer, 0) );
        GL( glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depthBuffer) );

        GLenum status;
        if ((status = glCheckFramebufferStatus(GL_FRAMEBUFFER)) != GL_FRAMEBUFFER_COMPLETE) {
            printf("ERROR OpenGLRenderTarget::init %d\n", status);
            return false;
        }
    }
    GL( glBindFramebuffer(GL_FRAMEBUFFER, 0) );

    _width = width;
    _height = height;

    return true;
}

void OpenGLFBRenderTarget::bind()
{
    GL( glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer) );
    GL( glViewport(0, 0, _width, _height) );
}

void OpenGLFBRenderTarget::unbind()
{
    GL( glBindFramebuffer(GL_FRAMEBUFFER, 0) );
}

bool OpenGLFBRenderTarget::blit(uint32_t dstX, uint32_t dstY, uint32_t width, uint32_t height, bool bindMainFB)
{
    GL( glBindFramebuffer(GL_READ_FRAMEBUFFER, _frameBuffer) );
	if (bindMainFB) {
		GL( glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0) );
	}
    GL( glEnable(GL_BLEND) );
    GL( glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) );

#define RENDERTARGET_SINGLE_BLIT
#ifdef RENDERTARGET_SINGLE_BLIT
    GL( glBlitFramebuffer(0, 0, _width, _height, dstX, dstY, dstX + width, dstY + height, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST) );
#else
    // If linear interpolation is needed we need to blit the color buffer first with
    // linear interpolation and then the depth buffer (and stencil if present)
    // with nearest
    glBlitFramebuffer(0, 0, _width, _height, dstX, dstY, dstX + width, dstY + height, GL_COLOR_BUFFER_BIT, GL_LINEAR);
    glBlitFramebuffer(0, 0, _width, _height, dstX, dstY, dstX + width, dstY + height, GL_DEPTH_BUFFER_BIT, GL_LINEAR);
#endif
    GL( glDisable(GL_BLEND) );
    GL( glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) );
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    return true;
}

void OpenGLFBRenderTarget::clear()
{
    GL( glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer) );
    GL( glClearColor(_r, _g, _b, _a) );
    GL( glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) );
}
