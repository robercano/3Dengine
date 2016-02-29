/**
 * @class	TextConsole
 * @brief	Implements an on-screen text console. It provides printf-like capabilities
 *          to print text to a render target. It allows to choose the color, the font type
 *          and the font size.
 *
 * @author	Roberto Sosa Cano
 */
#include <stdio.h>
#include <stdarg.h>

#include "TextConsole.hpp"
#include "NOAARenderTarget.hpp"

bool TextConsole::init(std::string &fontPath, uint32_t fontSize, glm::vec4 &color,
                       uint32_t width, uint32_t height)
{
    NOAARenderTarget *target = NOAARenderTarget::NewNOAARenderTarget();
	if (target == NULL) {
		return false;
	}

    if (target->init(width, height) != true) {
        NOAARenderTarget::DeleteNOAARenderTarget(target);
        return false;
    }

    /* Setup the font renderer */
    _font = TrueTypeFont::NewFont();

    if (_font->init(fontPath, fontSize) == false) {
        delete _font;
        NOAARenderTarget::DeleteNOAARenderTarget(target);
        return false;
    }

    _fontRenderer = FontRenderer::NewFontRenderer();
    if (_fontRenderer == NULL) {
        delete _font;
        NOAARenderTarget::DeleteNOAARenderTarget(target);
        return false;
    }

    _renderTarget = target;
    _fontRenderer->setFont(_font);
    _fontColor = color;
    _fontSize = fontSize;

    clear();

    return true;
}

void TextConsole::clear()
{
    _xPos = 5;
    _yPos = 5;

    _renderTarget->clear(0.0, 0.0, 0.0, 0.0);
}

int TextConsole::gprintf(const char *format, ...)
{
    char buffer[256];
    va_list args;
    va_start (args, format);
    vsprintf (buffer, format, args);
    va_end (args);

    /* TODO: add support for new lines and to wrap text around the
     * end of the render target */
    _fontRenderer->renderText(_xPos, _yPos, buffer, _fontColor, *_renderTarget);

    _yPos += _fontSize+4; // TODO: grab this information from the font renderer

    return 0;
}

void TextConsole::blit()
{
    _renderTarget->blit();
}
