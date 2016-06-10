/**
 * @class	OpenGLSSAARenderTarget
 * @brief	Render target for OpenGL. A render target allows to render objects to it
 *          instead of to the main screen. Then the target can be rendered to the main screen as
 *          a texture
 *
 *          The SSAA render target applies super sampled antia-aliasing
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#include "OpenGL.h"
#include "OpenGLSSAARenderTarget.hpp"
#include "Renderer.hpp"

OpenGLSSAARenderTarget::~OpenGLSSAARenderTarget()
{
    __( glDeleteBuffers(1, &_vertexBuffer) );
    __( glDeleteVertexArrays(1, &_vertexArray) );
    __( glDeleteTextures(1, &_colorBuffer) );
    __( glDeleteRenderbuffers(1, &_depthBuffer) );
    __( glDeleteFramebuffers(1, &_frameBuffer) );
}

bool OpenGLSSAARenderTarget::init(uint32_t width, uint32_t height, uint32_t factor)
{
    width *= factor;
    height *= factor;

    /* Texture buffer */
    __( glGenTextures(1, &_colorBuffer) );
    __( glBindTexture(GL_TEXTURE_2D, _colorBuffer) );
    {
        __( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR) );
        __( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR) );
        __( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE) );
        __( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE) );
        __( glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL) );
    }
    __( glBindTexture(GL_TEXTURE_2D, 0) );

    /* Depth buffer */
    __( glGenRenderbuffers(1, &_depthBuffer) );
    __( glBindRenderbuffer(GL_RENDERBUFFER, _depthBuffer) );
    {
        __( glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, width, height) );
    }
    __( glBindRenderbuffer(GL_RENDERBUFFER, 0) );

    /* Framebuffer to link everything together */
    __( glGenFramebuffers(1, &_frameBuffer) );
    __( glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer) );
    {
        __( glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _colorBuffer, 0) );
        __( glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depthBuffer) );

        GLenum status;
        if ((status = glCheckFramebufferStatus(GL_FRAMEBUFFER)) != GL_FRAMEBUFFER_COMPLETE) {
            printf("ERROR OpenGLRenderTarget::init %d\n", status);
            return false;
        }
    }
    __( glBindFramebuffer(GL_FRAMEBUFFER, 0) );

    /* Generate the render target surface */
    GLfloat verticesData[8] = {
        -1, -1,
        1, -1,
        -1,  1,
        1,  1,
    };

	__( glGenVertexArrays(1, &_vertexArray) );
	__( glBindVertexArray(_vertexArray) );
    {
        __( glGenBuffers(1, &_vertexBuffer) );
        __( glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer) );
        {
            __( glBufferData(GL_ARRAY_BUFFER, sizeof verticesData, verticesData, GL_STATIC_DRAW) );

            __( glEnableVertexAttribArray(0) );
            __( glVertexAttribPointer(
                    0,        // attribute
                    2,        // number of elements per vertex, here (x,y)
                    GL_FLOAT, // the type of each element
                    GL_FALSE, // take our values as-is
                    0,        // no extra data between each position
                    0         // offset of first element
                    ) );
        }
        __( glBindBuffer(GL_ARRAY_BUFFER, 0) );
    }
    __( glBindVertexArray(0) );

    _width = width;
    _height = height;
    _factor = factor;

    return true;
}

void OpenGLSSAARenderTarget::bind()
{
    __( glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer) );
    __( glViewport(0, 0, _width, _height) );
}

void OpenGLSSAARenderTarget::bindDepth()
{
    __( glBindTexture(GL_TEXTURE_2D, _depthBuffer) );
}

void OpenGLSSAARenderTarget::unbind()
{
    __( glBindFramebuffer(GL_FRAMEBUFFER, 0) );
}

bool OpenGLSSAARenderTarget::blit(uint32_t dstX, uint32_t dstY, uint32_t width, uint32_t height, bool bindMainFB)
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, _frameBuffer);
	if (bindMainFB) {
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	}
    glBlitFramebuffer(0, 0, _width, _height, dstX, dstY, width, height, GL_COLOR_BUFFER_BIT, GL_LINEAR);
    glBlitFramebuffer(0, 0, _width, _height, dstX, dstY, width, height, GL_DEPTH_BUFFER_BIT, GL_LINEAR);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    return true;
}

void OpenGLSSAARenderTarget::clear()
{
    __( glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer) );
    __( glClearColor(_r, _g, _b, _a) );
    __( glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) );
}
