/**
 * @class	OpenGLShadowMapRenderTarget
 * @brief	Render target for OpenGL. A render target allows to render objects to it
 *          instead of to the main screen. Then the target can be rendered to the main screen as
 *          a texture
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#include "OpenGL.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Logging.hpp"
#include "OpenGLShadowMapRenderTarget.hpp"
#include "Renderer.hpp"
#include "WindowManager.hpp"

using namespace Logging;

OpenGLShadowMapRenderTarget::~OpenGLShadowMapRenderTarget()
{
    __(glDeleteRenderbuffers(1, &_depthBuffer));
    __(glDeleteFramebuffers(1, &_frameBuffer));
}

bool OpenGLShadowMapRenderTarget::init(uint32_t width, uint32_t height, uint32_t maxSamples, uint32_t numTargets)
{
    (void)maxSamples;
    (void)numTargets;

    /* Depth buffer */
    __(glGenTextures(1, &_depthBuffer));
    __(glBindTexture(GL_TEXTURE_2D, _depthBuffer));
    {
        __(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
        __(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
        __(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        __(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
        __(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL));
        __(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE));

        __(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL));
    }
    __(glBindTexture(GL_TEXTURE_2D, 0));

    /* Framebuffer to link everything together */
    __(glGenFramebuffers(1, &_frameBuffer));
    __(glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer));
    {
        __(glDrawBuffer(GL_NONE));
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

    std::string error;
    if (_shader->use("utils/depth2color", error) == false) {
        log("ERROR loading shader utils/depth2color: %s\n", error.c_str());
        return false;
    }

    return true;
}

void OpenGLShadowMapRenderTarget::bind()
{
    __(glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer));
    __(glViewport(0, 0, _width, _height));
}

void OpenGLShadowMapRenderTarget::bindDepth() { __(glBindTexture(GL_TEXTURE_2D, _depthBuffer)); }
void OpenGLShadowMapRenderTarget::unbind() { __(glBindFramebuffer(GL_FRAMEBUFFER, 0)); }
bool OpenGLShadowMapRenderTarget::blit(uint32_t dstX, uint32_t dstY, uint32_t width, uint32_t height, uint32_t target, bool bindMainFB)
{
    (void)target;

    /* Setup the viewport */
    __(glViewport(dstX, dstY, width, height));

    /* Bind the target texture */
    if (bindMainFB) {
        __(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    }

    __(glViewport(dstX, dstY, width, height));
    __(glActiveTexture(GL_TEXTURE0));
    __(glBindTexture(GL_TEXTURE_2D, _depthBuffer));

    /* Tell the shader which texture unit to use */
    _shader->attach();
    _shader->setUniformTexture2D("u_depthMap", 0);

    __(glDisable(GL_DEPTH_TEST));
    //__( glEnable(GL_BLEND) );
    //__( glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) );

    __(glBindVertexArray(_vertexArray));
    {
        __(glDrawArrays(GL_TRIANGLE_STRIP, 0, 4));
    }
    __(glBindVertexArray(0));

    //__( glDisable(GL_BLEND) );
    __(glEnable(GL_DEPTH_TEST));

    _shader->detach();

    return true;
}

void OpenGLShadowMapRenderTarget::clear()
{
    __(glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer));
    __(glClearColor(_r, _g, _b, _a));
    __(glClearDepth(1.0f));
    __(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}
