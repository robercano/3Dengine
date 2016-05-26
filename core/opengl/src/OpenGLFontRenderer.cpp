/*
 * @class FontRenderer
 * @brief Uses a TrueType font to render text on a RenderTarget
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#include <string.h>
#include "OpenGLFontRenderer.hpp"
#include "OpenGL.h"
#include "MathUtils.h"
#include "Renderer.hpp"
#include "glm/gtx/transform.hpp"

OpenGLFontRenderer::OpenGLFontRenderer() : _font(NULL),  _glyphTextures()
{
}

OpenGLFontRenderer::~OpenGLFontRenderer()
{
    if (_font != NULL) {
        glDeleteTextures(GL_FONT_RENDERER_NUM_GLYPHS, _glyphTextures);
    }
}

bool OpenGLFontRenderer::setFont(TrueTypeFont *font)
{
	uint32_t i;
    GLubyte *textureData = NULL;
    uint32_t lastTextureSize = 0;

    _font = font;

    /* Generate all textures and vertex arrays for the glyphs */
    GL( glGenTextures(GL_FONT_RENDERER_NUM_GLYPHS, _glyphTextures) );
    GL( glGenVertexArrays(GL_FONT_RENDERER_NUM_GLYPHS, _glyphVAOs) );
    GL( glGenBuffers(GL_FONT_RENDERER_NUM_GLYPHS, _glyphBuffers) );
    for (i=0; i<GL_FONT_RENDERER_NUM_GLYPHS; ++i) {
        const uint8_t *buffer = NULL;
        uint32_t width = 0, height = 0, offsetLeft, offsetTop, advance;

        buffer = _font->getBitmap(i, width, height, offsetLeft, offsetTop, advance);

        GL( glBindTexture(GL_TEXTURE_2D, _glyphTextures[i]) );
        {
            /* Swizzle mask to emulate GL_LUMINANCE_ALPHA behaviour,
             * just for fun */
            GLint swizzleMask[] = {GL_RED, GL_RED, GL_RED, GL_RED};

            GL( glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR) );
            GL( glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR) );
            GL( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE) );
            GL( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE) );

            GL( glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask) );

            GL( glPixelStorei(GL_UNPACK_ALIGNMENT, 1) );

            GL( glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, buffer) );
        }
        GL( glBindTexture(GL_TEXTURE_2D, 0) );

        /* Bind the vertex array for this glyph */
        GL( glBindVertexArray(_glyphVAOs[i]) );
        {
            GL( glBindBuffer(GL_ARRAY_BUFFER, _glyphBuffers[i]) );
            {
                GLfloat verticesData[] = {
                    0.0f, 0.0f, // xy coord
                    0.0f, 0.0f, // uv coord
                    0.0f, (GLfloat)height,
                    0.0f, 1.0f,
                    (GLfloat)width, 0.0f,
                    1.0, 0.0f,
                    (GLfloat)width, (GLfloat)height,
                    1.0f, 1.0f
                };

                GL( glBufferData(GL_ARRAY_BUFFER, sizeof verticesData, verticesData, GL_STATIC_DRAW) );

                GL( glEnableVertexAttribArray(0) );
                GL( glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), 0) );

                GL( glEnableVertexAttribArray(1) );
                GL( glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), reinterpret_cast<void*>(2*sizeof(GLfloat))) );
            }
            GL( glBindBuffer(GL_ARRAY_BUFFER, 0) );
        }
        GL( glBindVertexArray(0) );
    }

    /* Create the shader */
    _shader = Shader::New();

    std::string error;
    if (_shader->use("text/glyph", error) == false) {
		printf("ERROR compiling shader text/glyph: %s\n", error.c_str());
		return 1;
	}

    delete[] textureData;

    return false;
}

bool OpenGLFontRenderer::renderText(uint32_t x, uint32_t y, std::string &text, glm::vec4 &color, RenderTarget &target)
{
    return renderText(x, y, text.c_str(), color, target);
}

bool OpenGLFontRenderer::renderText(uint32_t x, uint32_t y, const char *text, glm::vec4 &color, RenderTarget &target)
{
    GL( glEnable(GL_BLEND) );
    GL( glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) );

    target.bind();

    float renderWidth  = (float)target.getWidth();
    float renderHeight = (float)target.getHeight();

    GL( glActiveTexture(GL_TEXTURE0) );

    /* Setup the constant values for the shader */
    _shader->attach();

    /* The glyphs are created in window coordinates so we don't
     * need the glyphs width and height here. This matrix scales
     * to (0.0, 1.0) on x and y, then transforms into NDC */
    glm::mat4 glyphTransform( 2.0f/renderWidth, 0.0f, 0.0f, 0.0f,
                              0.0f, -2.0f/renderHeight, 0.0f, 0.0f,
                              0.0f, 0.0f, 1.0f, 0.0f,
                              -1.0f, 1.0f, 0.0f, 1.0f);

    _shader->setUniformMat4("glyphTransform", &glyphTransform);
    _shader->setUniformTexture2D("glyph", 0);
    _shader->setUniformVec4("color", color);

    GL( glDisable(GL_DEPTH_TEST) );

    uint32_t textLength = strlen(text);
    for (size_t i=0; i<textLength; ++i) {
        /* Get the size of the glyph */
        uint32_t glyphWidth, glyphHeight, offsetLeft, offsetTop, advance;

        _font->getBitmap(text[i], glyphWidth, glyphHeight, offsetLeft, offsetTop, advance);

        /* Adjust the coordinates to take into account the bearings */
        glm::vec2 glyphPos = glm::vec2((float)(x + offsetLeft), float(y + offsetTop));
        _shader->setUniformVec2("glyphPos", glyphPos);

        /* Bind the target texture */
        GL( glBindTexture(GL_TEXTURE_2D, _glyphTextures[text[i]]) );
        {
            GL( glBindVertexArray(_glyphVAOs[text[i]]) );
            {
                GL( glDrawArrays(GL_TRIANGLE_STRIP, 0, 4) );
            }
        }

        x += advance;
    }

    GL( glEnable(GL_DEPTH_TEST) );

    _shader->detach();
    target.unbind();

    GL( glDisable(GL_BLEND) );

    return false;
}
