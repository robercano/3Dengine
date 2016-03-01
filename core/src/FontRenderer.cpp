/**
 * @class FontRenderer
 * @brief Uses a TrueType font to render text on a RenderTarget
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#include "FontRenderer.hpp"
#include "OpenGLFontRenderer.hpp"

FontRenderer *FontRenderer::NewFontRenderer()
{
    return new OpenGLFontRenderer();
}

void FontRenderer::DeleteFontRenderer(FontRenderer *fontRenderer)
{
    delete fontRenderer;
}
