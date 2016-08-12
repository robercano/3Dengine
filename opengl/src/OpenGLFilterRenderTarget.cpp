/**
 * @class	OpenGLFilterRenderTarget
 * @brief	Render target for OpenGL. A render target allows to render objects to it
 *          instead of to the main screen. Then the target can be rendered to the main screen as
 *          a texture
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#include "OpenGL.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Logging.hpp"
#include "OpenGLFilterRenderTarget.hpp"
#include "Renderer.hpp"
#include "WindowManager.hpp"

using namespace Logging;

OpenGLFilterRenderTarget::~OpenGLFilterRenderTarget()
{
    delete _shader;

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

bool OpenGLFilterRenderTarget::init(uint32_t width, uint32_t height, uint32_t maxSamples, uint32_t numTargets)
{
    (void)maxSamples;

    if (numTargets <= 0 || numTargets > GL_MAX_COLOR_ATTACHMENTS) {
        log("Number of targets for render target (%d) not supported. Max. is %d\n", numTargets, GL_MAX_COLOR_ATTACHMENTS);
    }

    _numTargets = numTargets;

    /* Allocate the color buffers IDs */
    _colorBuffer = new GLuint[_numTargets];

    /* Retrieve maximum anisotropy filter value */
    GLfloat fLargest;
    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &fLargest);

    /* TODO: Limit it to 4, this should be configurable */
    if (fLargest > 4.0) {
        fLargest = 4.0;
    }

    /* Texture buffer */
    __(glGenTextures(_numTargets, _colorBuffer));
    for (int i = 0; i < _numTargets; ++i) {
        __(glBindTexture(GL_TEXTURE_2D, _colorBuffer[i]));
        {
            __(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
            __(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
            __(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
            __(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
            __(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, fLargest));

            __(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL));
        }
    }
    __(glBindTexture(GL_TEXTURE_2D, 0));

    /* Depth buffer */
    __(glGenTextures(1, &_depthBuffer));
    __(glBindTexture(GL_TEXTURE_2D, _depthBuffer));
    {
        __(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
        __(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
        __(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        __(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

        __(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL));
    }
    __(glBindTexture(GL_TEXTURE_2D, 0));

    /* Framebuffer to link everything together */
    _attachments = new GLuint[_numTargets];

    __(glGenFramebuffers(1, &_frameBuffer));
    __(glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer));
    {
        /* Attach all color buffers */
        for (int i = 0; i < _numTargets; ++i) {
            _attachments[i] = GL_COLOR_ATTACHMENT0 + i;
            __(glFramebufferTexture2D(GL_FRAMEBUFFER, _attachments[i], GL_TEXTURE_2D, _colorBuffer[i], 0));
        }
        __(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _depthBuffer, 0));

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

    /* Create the shader */
    _shader = Shader::New();

    _width = width;
    _height = height;

    return customInit();
}

void OpenGLFilterRenderTarget::bind()
{
    __(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _frameBuffer));
    __(glDrawBuffers(_numTargets, _attachments));
    __(glViewport(0, 0, _width, _height));
}

void OpenGLFilterRenderTarget::bindDepth() { __(glBindTexture(GL_TEXTURE_2D, _depthBuffer)); }
void OpenGLFilterRenderTarget::unbind() { __(glBindFramebuffer(GL_FRAMEBUFFER, 0)); }
bool OpenGLFilterRenderTarget::blit(uint32_t dstX, uint32_t dstY, uint32_t width, uint32_t height, uint32_t target, bool bindMainFB)
{
    if (target < 0 || target >= _numTargets) {
        log("ERROR wrong target number %d in OpenGLFilterRenderTarget::blit, max. is %d\n", target, _numTargets - 1);
        return false;
    }

    /* Bind the target texture */
    if (bindMainFB) {
        __(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0));
    }

    /* Set the rendering mode */
    __(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
    __(glDisable(GL_LINE_SMOOTH));
    __(glEnable(GL_CULL_FACE));
    __(glDisable(GL_BLEND));

    __(glViewport(dstX, dstY, width, height));
    __(glActiveTexture(GL_TEXTURE0));
    __(glBindTexture(GL_TEXTURE_2D, _colorBuffer[target]));

    /* Tell the shader which texture unit to use */
    _shader->attach();
    _shader->setUniformTexture2D("fbo_texture", 0);

    setCustomParams();

    __(glBindVertexArray(_vertexArray));
    {
        __(glDrawArrays(GL_TRIANGLE_STRIP, 0, 4));
    }
    __(glBindVertexArray(0));

    unsetCustomParams();

    _shader->detach();

    return true;
}

void OpenGLFilterRenderTarget::clear()
{
    __(glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer));
    __(glDrawBuffers(_numTargets, _attachments));
    __(glClearColor(_r, _g, _b, _a));
    __(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}
