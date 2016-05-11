/**
 * @class	OpenGLShadowMapRenderTarget
 * @brief	Render target for OpenGL. A render target allows to render objects to it
 *          instead of to the main screen. Then the target can be rendered to the main screen as
 *          a texture
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#include <glm/gtc/matrix_transform.hpp>
#include "OpenGL.h"
#include "OpenGLShadowMapRenderTarget.hpp"
#include "Renderer.hpp"
#include "WindowManager.hpp"

OpenGLShadowMapRenderTarget::~OpenGLShadowMapRenderTarget()
{
    GL( glDeleteRenderbuffers(1, &_depthBuffer) );
    GL( glDeleteFramebuffers(1, &_frameBuffer) );
}

bool OpenGLShadowMapRenderTarget::init(uint32_t width, uint32_t height, uint32_t maxSamples)
{
	(void)maxSamples;

    /* Depth buffer */
    GL( glGenTextures(1, &_depthBuffer) );
    GL( glBindTexture(GL_TEXTURE_2D, _depthBuffer) );
    {
        GL( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST) );
        GL( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST) );
        GL( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE) );
        GL( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE) );

        GL( glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL) );
    }
    GL( glBindTexture(GL_TEXTURE_2D, 0) );

    /* Framebuffer to link everything together */
    GL( glGenFramebuffers(1, &_frameBuffer) );
    GL( glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer) );
    {
        GL( glDrawBuffer(GL_NONE) );
        GL( glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _depthBuffer, 0) );

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
    _shader = Shader::New();

    _width = width;
    _height = height;

	std::string error;
	if (_shader->use("utils/depth2color", error) == false) {
		printf("ERROR loading shader utils/depth2color: %s\n", error.c_str());
		return false;
	}

	return true;
}

void OpenGLShadowMapRenderTarget::bind()
{
    GL( glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer) );
    GL( glViewport(0, 0, _width, _height) );
}

void OpenGLShadowMapRenderTarget::bindDepth()
{
    GL( glBindTexture(GL_TEXTURE_2D, _depthBuffer) );
}

void OpenGLShadowMapRenderTarget::unbind()
{
    GL( glBindFramebuffer(GL_FRAMEBUFFER, 0) );
}

bool OpenGLShadowMapRenderTarget::blit(uint32_t dstX, uint32_t dstY, uint32_t width, uint32_t height, bool bindMainFB)
{
	/* Setup the viewport */
    GL( glViewport(dstX, dstY, width, height) );

    /* Bind the target texture */
	if (bindMainFB) {
		GL( glBindFramebuffer(GL_FRAMEBUFFER, 0) );
	}

    GL( glViewport(dstX, dstY, width, height) );
    GL( glActiveTexture(GL_TEXTURE0) );
    GL( glBindTexture(GL_TEXTURE_2D, _depthBuffer) );

    /* Tell the shader which texture unit to use */
    _shader->attach();
    _shader->setUniformTexture2D("u_depthMap", 0);

	GL( glDisable(GL_DEPTH_TEST) );
	//GL( glEnable(GL_BLEND) );
	//GL( glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) );

    GL( glBindVertexArray(_vertexArray) );
    {
        GL( glDrawArrays(GL_TRIANGLE_STRIP, 0, 4) );
    }
    GL( glBindVertexArray(0) );

	//GL( glDisable(GL_BLEND) );
	GL( glEnable(GL_DEPTH_TEST) );

    _shader->detach();

    return true;
}

void OpenGLShadowMapRenderTarget::clear()
{
    GL( glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer) );
    GL( glClearColor(_r, _g, _b, _a) );
    GL( glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) );
}
