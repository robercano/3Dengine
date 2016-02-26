/**
 * @class	OpenGLRenderTarget
 * @brief	Render target for OpenGL. A render target allows to render objects to it
 *          instead of to the main screen. Then the target can be rendered to the main screen as
 *          a texture
 *
 * @author	Roberto Sosa Cano (http://www.robertocano.es)
 */
#include "OpenGL.h"
#include "OpenGLMSAARenderTarget.hpp"
#include "Renderer.hpp"

bool OpenGLMSAARenderTarget::init(uint32_t width, uint32_t height, uint32_t samples)
{
    glActiveTexture(GL_TEXTURE0);

    /* Texture buffer */
    GL( glGenTextures(1, &_colorBuffer) );
    GL( glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, _colorBuffer) );
    {
        GL( glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGBA8, width, height, GL_TRUE) );
    }
    GL( glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0) );

    /* Depth buffer */
    GL( glGenRenderbuffers(1, &_depthBuffer) );
    GL( glBindRenderbuffer(GL_RENDERBUFFER, _depthBuffer) );
    {
        GL( glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH_COMPONENT32, width, height) );
    }
    GL( glBindRenderbuffer(GL_RENDERBUFFER, 0) );

    /* Framebuffer to link everything together */
    GL( glGenFramebuffers(1, &_frameBuffer) );
    GL( glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer) );
    {
        GL( glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, _colorBuffer, 0) );
        GL( glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depthBuffer) );

        GLenum status;
        if ((status = glCheckFramebufferStatus(GL_FRAMEBUFFER)) != GL_FRAMEBUFFER_COMPLETE) {
            printf("ERROR OpenGLRenderTarget::init %d\n", status);
            return false;
        }
    }
    GL( glBindFramebuffer(GL_FRAMEBUFFER, 0) );

    /* Generate the render target surface */
    GLfloat verticesData[8] = {
        -1, -1,
        1, -1,
        -1,  1,
        1,  1,
    };

	GL( glGenVertexArrays(1, &_vertexArray) );
	GL( glBindVertexArray(_vertexArray) );
    {
        GL( glGenBuffers(1, &_vertexBuffer) );
        GL( glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer) );
        {
            GL( glBufferData(GL_ARRAY_BUFFER, sizeof verticesData, verticesData, GL_STATIC_DRAW) );

            GL( glEnableVertexAttribArray(0) );
            GL( glVertexAttribPointer(
                    0,        // attribute
                    2,        // number of elements per vertex, here (x,y)
                    GL_FLOAT, // the type of each element
                    GL_FALSE, // take our values as-is
                    0,        // no extra data between each position
                    0         // offset of first element
                    ) );
        }
        GL( glBindBuffer(GL_ARRAY_BUFFER, 0) );
    }
    GL( glBindVertexArray(0) );

    /* Create the shader */
    _shader = Renderer::GetRenderer()->newShader();

	std::string error;
	if (_shader->loadVertexShader("data/shaders/effects/noeffect.vert", error) == false) {
		printf("ERROR compiling vertex shader: %s\n", error.c_str());
		return 1;
	}
	if (_shader->loadFragmentShader("data/shaders/effects/noeffect.frag", error) == false) {
		printf("ERROR compiling fragment shader: %s\n", error.c_str());
		return 1;
	}
	if (_shader->linkProgram(error) == false) {
		printf("ERROR linking shader: %s\n", error.c_str());
		return 1;
	}

    _width = width;
    _height = height;
    return true;
}

void OpenGLMSAARenderTarget::bind()
{
    GL( glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer) );
    GL( glEnable(GL_MULTISAMPLE) );
    GL( glViewport(0, 0, _width, _height) );
}

void OpenGLMSAARenderTarget::unbind()
{
    GL( glDisable(GL_MULTISAMPLE) );
    GL( glBindFramebuffer(GL_FRAMEBUFFER, 0) );
}

uint32_t OpenGLMSAARenderTarget::getMaxSamples()
{
    GLint samples;
    GL( glGetIntegerv(GL_MAX_SAMPLES, &samples) );
    return (uint32_t)samples;
}

bool OpenGLMSAARenderTarget::blit(uint32_t dstX, uint32_t dstY, uint32_t width, uint32_t height)
{
    if ((width-dstX) != _width || (height-dstY) != _height) {
        fprintf(stderr, "ERROR in OpenGLMSAARenderTarget::blit() different size for src and dest\n");
        return false;
    }
    GL( glBindFramebuffer(GL_READ_FRAMEBUFFER, _frameBuffer) );
    GL( glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0) );
    /* As this is a multi-sample buffer source and destination rectangles must be of
     * the same size, thus we always use GL_NEAREST and blit color and depth at the same time */
    GL( glBlitFramebuffer(0, 0, _width, _height, dstX, dstY, width, height, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST) );
    GL( glBindFramebuffer(GL_READ_FRAMEBUFFER, 0) );
    return true;
}
