/**
 * @class FontRenderer
 * @brief Uses a TrueType font to render text on a RenderTarget
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include <stdint.h>
#include <glm/glm.hpp>
#include <string>
#include "FontRenderer.hpp"
#include "OpenGL.h"
#include "Shader.hpp"

class OpenGLFontRenderer : public FontRenderer
{
#define GL_FONT_RENDERER_NUM_GLYPHS 128
  public:
    OpenGLFontRenderer();
    ~OpenGLFontRenderer();

    bool setFont(TrueTypeFont *font);
    bool renderText(uint32_t x, uint32_t y, std::string &text, glm::vec4 &color, RenderTarget &target);
    bool renderText(uint32_t x, uint32_t y, const char *text, glm::vec4 &color, RenderTarget &target);

  private:
    GLuint _glyphTextures[GL_FONT_RENDERER_NUM_GLYPHS];
    GLuint _glyphVAOs[GL_FONT_RENDERER_NUM_GLYPHS];
    GLuint _glyphBuffers[GL_FONT_RENDERER_NUM_GLYPHS];
    TrueTypeFont *_font;
    Shader *_shader;
};
