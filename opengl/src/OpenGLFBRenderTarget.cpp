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
#include "OpenGL.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Logging.hpp"
#include "OpenGLFBRenderTarget.hpp"
#include "Renderer.hpp"
#include "WindowManager.hpp"

using namespace Logging;

OpenGLFBRenderTarget::~OpenGLFBRenderTarget()
{
    for (unsigned int i = 0; i < _numTargets; ++i) {
        __(glDeleteTextures(1, &_colorBuffer[i]));
    }
    delete[] _colorBuffer;
    _colorBuffer = NULL;

    delete[] _attachments;
    _attachments = NULL;

    __(glDeleteRenderbuffers(1, &_depthBuffer));
    __(glDeleteFramebuffers(1, &_frameBuffer));
}

bool OpenGLFBRenderTarget::init(uint32_t width, uint32_t height, uint32_t maxSamples, uint32_t numTargets)
{
    (void)maxSamples;

    if (numTargets <= 0 || numTargets > GL_MAX_COLOR_ATTACHMENTS) {
        log("Number of targets for render target (%d) not supported. Max. is %d\n", numTargets, GL_MAX_COLOR_ATTACHMENTS);
    }

    _numTargets = numTargets;

    /* Allocate the color buffers IDs */
    _colorBuffer = new GLuint[_numTargets];

    /* Texture buffer */
    __(glGenTextures(_numTargets, _colorBuffer));
    for (unsigned int i = 0; i < _numTargets; ++i) {
        __(glBindTexture(GL_TEXTURE_2D, _colorBuffer[i]));
        {
            __(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
            __(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
            __(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
            __(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
            __(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL));
        }
    }
    __(glBindTexture(GL_TEXTURE_2D, 0));

    /* Depth buffer */
    __(glGenRenderbuffers(1, &_depthBuffer));
    __(glBindRenderbuffer(GL_RENDERBUFFER, _depthBuffer));
    {
        __(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, width, height));
    }
    __(glBindRenderbuffer(GL_RENDERBUFFER, 0));

    /* Framebuffer to link everything together */
    _attachments = new GLuint[_numTargets];

    __(glGenFramebuffers(1, &_frameBuffer));
    __(glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer));
    {
        /* Attach all color buffers */
        for (unsigned int i = 0; i < _numTargets; ++i) {
            _attachments[i] = GL_COLOR_ATTACHMENT0 + i;
            __(glFramebufferTexture2D(GL_FRAMEBUFFER, _attachments[i], GL_TEXTURE_2D, _colorBuffer[i], 0));
        }
        __(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depthBuffer));

        GLenum status;
        if ((status = glCheckFramebufferStatus(GL_FRAMEBUFFER)) != GL_FRAMEBUFFER_COMPLETE) {
            log("ERROR OpenGLRenderTarget::init %d\n", status);
            return false;
        }
    }
    __(glBindFramebuffer(GL_FRAMEBUFFER, 0));

    _width = width;
    _height = height;

    return true;
}

void OpenGLFBRenderTarget::bind()
{
    __(glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer));
    __(glDrawBuffers(_numTargets, _attachments));
    __(glViewport(0, 0, _width, _height));
}

void OpenGLFBRenderTarget::bindDepth() { __(glBindTexture(GL_TEXTURE_2D, _depthBuffer)); }
void OpenGLFBRenderTarget::unbind() { __(glBindFramebuffer(GL_FRAMEBUFFER, 0)); }
bool OpenGLFBRenderTarget::blit(uint32_t dstX, uint32_t dstY, uint32_t width, uint32_t height, uint32_t target, bool bindMainFB)
{
    if (target >= _numTargets) {
        log("ERROR wrong target number %d in OpenGLFilterRenderTarget::blit, max. is %d\n", target, _numTargets - 1);
        return false;
    }

    __(glBindFramebuffer(GL_READ_FRAMEBUFFER, _frameBuffer));
    __(glReadBuffer(GL_COLOR_ATTACHMENT0 + target));

    if (bindMainFB) {
        __(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0));
    }
    __(glEnable(GL_BLEND));
    __(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

#define RENDERTARGET_SINGLE_BLIT
#ifdef RENDERTARGET_SINGLE_BLIT
    __(glBlitFramebuffer(0, 0, _width, _height, dstX, dstY, dstX + width, dstY + height, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT,
                         GL_NEAREST));
#else
    // If linear interpolation is needed we need to blit the color buffer first with
    // linear interpolation and then the depth buffer (and stencil if present)
    // with nearest
    glBlitFramebuffer(0, 0, _width, _height, dstX, dstY, dstX + width, dstY + height, GL_COLOR_BUFFER_BIT, GL_LINEAR);
    glBlitFramebuffer(0, 0, _width, _height, dstX, dstY, dstX + width, dstY + height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
#endif
    __(glDisable(GL_BLEND));
    __(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    return true;
}

void OpenGLFBRenderTarget::clear()
{
    __(glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer));
    __(glDrawBuffers(_numTargets, _attachments));
    __(glClearColor(_r, _g, _b, _a));
    __(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}
