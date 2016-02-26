/**
 * @class FontRenderer
 * @brief Uses a TrueType font to render text on a RenderTarget
 *
 * @author Roberto Cano
 */
#include "FontRenderer.hpp"
#include "OpenGLFontRenderer.hpp"

FontRenderer *FontRenderer::_fontRenderer = NULL;

FontRenderer *FontRenderer::GetFontRenderer()
{
    if (_fontRenderer == NULL) {
        _fontRenderer = new OpenGLFontRenderer();
    }
    return _fontRenderer;
}
