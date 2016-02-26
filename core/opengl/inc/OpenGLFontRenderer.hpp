/**
 * @class FontRenderer
 * @brief Uses a TrueType font to render text on a RenderTarget
 *
 * @author Roberto Cano
 */
#pragma once

#include <string>
#include <stdint.h>
#include "FontRenderer.hpp"
#include "OpenGL.h"

class OpenGLFontRenderer : public FontRenderer
{
#define GL_FONT_RENDERER_NUM_GLYPHS 128
    public:
        OpenGLFontRenderer();
        ~OpenGLFontRenderer();

        bool setFont(TrueTypeFont *font);
        bool renderText(std::string &text, RenderTarget &target);

	private:
        GLuint _glyphTextures[GL_FONT_RENDERER_NUM_GLYPHS];
        TrueTypeFont *_font;
};
