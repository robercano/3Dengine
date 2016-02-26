/**
 * @class FontRenderer
 * @brief Uses a TrueType font to render text on a RenderTarget
 *
 * @author Roberto Cano
 */
#include "OpenGLFontRenderer.hpp"
#include "OpenGL.h"
#include "MathUtils.h"

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
    uint32_t i, j, k;
    GLubyte *textureData = NULL;
    uint32_t lastTextureSize = 0;

    _font = font;

    /* Generate all textures for the glyphs */
    glGenTextures(GL_FONT_RENDERER_NUM_GLYPHS, _glyphTextures);
    for (i=0; i<GL_FONT_RENDERER_NUM_GLYPHS; ++i) {
        const uint8_t *buffer = NULL;
        uint32_t width = 0;
        uint32_t height = 0;

        buffer = _font->getBitmap(i, width, height);
        if (buffer == NULL) {
            _font = NULL;
            delete[] textureData;
            glDeleteTextures(GL_FONT_RENDERER_NUM_GLYPHS, _glyphTextures);
            return false;
        }

        uint32_t textureWidth  = clp2(width);
        uint32_t textureHeight = clp2(height);

        /* Generate texture data only for luminance and alpha */
        if ((2 * textureWidth * textureHeight) != lastTextureSize) {
            delete[] textureData;
            lastTextureSize = 2 * textureWidth * textureHeight;
            textureData = new GLubyte[lastTextureSize];
        }

        /* Copy the bitmap and fill the gaps */
        for (k=0; k<textureHeight; ++k) {
            for (j=0; j<textureWidth; ++j) {
                uint32_t offset = k * textureWidth * 2 + j;
                if (j <= width) {
                   textureData[offset    ] = buffer[offset];
                   textureData[offset + 1] = buffer[offset];
                } else {
                   textureData[offset    ] = 0;
                   textureData[offset + 1] = 0;
                }
            }
        }

        glBindTexture(GL_TEXTURE_2D, _glyphTextures[i]);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, textureData);
    }

    delete[] textureData;

    return false;
}

bool OpenGLFontRenderer::renderText(std::string &text, RenderTarget &target)
{
    return false;
}
