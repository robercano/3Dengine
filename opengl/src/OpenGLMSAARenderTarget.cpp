/**
 * @class	OpenGLRenderTarget
 * @brief	Render target for OpenGL. A render target allows to render objects to it
 *          instead of to the main screen. Then the target can be rendered to the main screen as
 *          a texture
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#include "OpenGL.h"
#include "Logging.hpp"
#include "OpenGLMSAARenderTarget.hpp"
#include "Renderer.hpp"

using namespace Logging;

OpenGLMSAARenderTarget::~OpenGLMSAARenderTarget()
{
    __(glDeleteBuffers(1, &_vertexBuffer));
    __(glDeleteVertexArrays(1, &_vertexArray));

    for (int i = 0; i < _numTargets; ++i) {
        __(glDeleteTextures(1, &_colorBuffer[i]));
    }
    delete[] _colorBuffer;
    _colorBuffer = NULL;

    delete[] _attachments;
    _attachments = NULL;

    __(glDeleteRenderbuffers(1, &_depthBuffer));
    __(glDeleteFramebuffers(1, &_frameBuffer));
}

bool OpenGLMSAARenderTarget::init(uint32_t width, uint32_t height, uint32_t numTargets, uint32_t samples)
{
    if (numTargets <= 0 || numTargets > GL_MAX_COLOR_ATTACHMENTS) {
        log("Number of targets for render target (%d) not supported. Max. is %d\n", numTargets, GL_MAX_COLOR_ATTACHMENTS);
    }

    _numTargets = numTargets;

    /* Allocate the color buffers IDs */
    _colorBuffer = new GLuint[_numTargets];

    /* Texture buffer */
    __(glGenTextures(_numTargets, _colorBuffer));
    for (int i = 0; i < _numTargets; ++i) {
        __(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, _colorBuffer[i]));
        {
            __(glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGBA8, width, height, GL_TRUE));
        }
    }
    __(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0));

    /* Depth buffer */
    __(glGenRenderbuffers(1, &_depthBuffer));
    __(glBindRenderbuffer(GL_RENDERBUFFER, _depthBuffer));
    {
        __(glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH_COMPONENT32, width, height));
    }
    __(glBindRenderbuffer(GL_RENDERBUFFER, 0));

    /* Framebuffer to link everything together */
    _attachments = new GLuint[_numTargets];

    /* Framebuffer to link everything together */
    __(glGenFramebuffers(1, &_frameBuffer));
    __(glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer));
    {
        /* Attach all color buffers */
        for (int i = 0; i < _numTargets; ++i) {
            _attachments[i] = GL_COLOR_ATTACHMENT0 + i;
            __(glFramebufferTexture2D(GL_FRAMEBUFFER, _attachments[i], GL_TEXTURE_2D_MULTISAMPLE, _colorBuffer[i], 0));
        }
        __(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depthBuffer));

        GLenum status;
        if ((status = glCheckFramebufferStatus(GL_FRAMEBUFFER)) != GL_FRAMEBUFFER_COMPLETE) {
            log("ERROR OpenGLRenderTarget::init %d\n", status);
            return false;
        }
    }
    __(glBindFramebuffer(GL_FRAMEBUFFER, 0));

    /* Generate the render target surface */
    GLfloat verticesData[8] = {
        -1, -1, 1, -1, -1, 1, 1, 1,
    };

    __(glGenVertexArrays(1, &_vertexArray));
    __(glBindVertexArray(_vertexArray));
    {
        __(glGenBuffers(1, &_vertexBuffer));
        __(glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer));
        {
            __(glBufferData(GL_ARRAY_BUFFER, sizeof verticesData, verticesData, GL_STATIC_DRAW));

            __(glEnableVertexAttribArray(0));
            __(glVertexAttribPointer(0,         // attribute
                                     2,         // number of elements per vertex, here (x,y)
                                     GL_FLOAT,  // the type of each element
                                     GL_FALSE,  // take our values as-is
                                     0,         // no extra data between each position
                                     0          // offset of first element
                                     ));
        }
        __(glBindBuffer(GL_ARRAY_BUFFER, 0));
    }
    __(glBindVertexArray(0));

    _width = width;
    _height = height;
    return true;
}

void OpenGLMSAARenderTarget::bind()
{
    __(glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer));
    __(glDrawBuffers(_numTargets, _attachments));
    __(glEnable(GL_MULTISAMPLE));
    __(glViewport(0, 0, _width, _height));
}

void OpenGLMSAARenderTarget::bindDepth() { __(glBindTexture(GL_TEXTURE_2D, _depthBuffer)); }
void OpenGLMSAARenderTarget::unbind()
{
    __(glDisable(GL_MULTISAMPLE));
    __(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

uint32_t OpenGLMSAARenderTarget::getMaxSamples()
{
    GLint samples;
    __(glGetIntegerv(GL_MAX_SAMPLES, &samples));
    return (uint32_t)samples;
}

bool OpenGLMSAARenderTarget::blit(uint32_t dstX, uint32_t dstY, uint32_t width, uint32_t height, uint32_t target, bool bindMainFB)
{
    if (target >= _numTargets) {
        log("ERROR wrong target number %d in OpenGLFilterRenderTarget::blit, max. is %d\n", target, _numTargets - 1);
        return false;
    }

    if ((width - dstX) != _width || (height - dstY) != _height) {
        fprintf(stderr, "ERROR in OpenGLMSAARenderTarget::blit() different size for src and dest\n");
        return false;
    }

    __(glBindFramebuffer(GL_READ_FRAMEBUFFER, _frameBuffer));
    __(glReadBuffer(GL_COLOR_ATTACHMENT0 + target));

    if (bindMainFB) {
        __(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0));
    }

    /* As this is a multi-sample buffer source and destination rectangles must be of
     * the same size, thus we always use GL_NEAREST and blit color and depth at the same time */
    __(glBlitFramebuffer(0, 0, _width, _height, dstX, dstY, width, height, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST));
    __(glBindFramebuffer(GL_READ_FRAMEBUFFER, 0));
    return true;
}

void OpenGLMSAARenderTarget::clear()
{
    __(glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer));
    __(glDrawBuffers(_numTargets, _attachments));
    __(glClearColor(_r, _g, _b, _a));
    __(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}
