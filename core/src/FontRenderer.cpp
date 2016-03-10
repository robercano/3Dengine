/**
 * @class FontRenderer
 * @brief Uses a TrueType font to render text on a RenderTarget
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#include "FontRenderer.hpp"
#include "OpenGLFontRenderer.hpp"

FontRenderer *FontRenderer::New(void)
{
    return new OpenGLFontRenderer();
}

void FontRenderer::Delete(FontRenderer *fontRenderer)
{
    delete fontRenderer;
}
