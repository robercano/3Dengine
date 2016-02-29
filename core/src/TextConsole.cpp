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

bool TextConsole::init(std::string &fontPath, uint32_t fontSize, glm::vec4 &color)
{
    /* Setup the font renderer */
    _font = TrueTypeFont::NewFont();

    if (_font->init(fontPath, fontSize) == false) {
        delete _font;
        return false;
    }

    _fontRenderer = FontRenderer::NewFontRenderer();
    if (_fontRenderer == NULL) {
        printf("ERROR getting font renderer\n");
        return false;
    }

    _fontRenderer->setFont(_font);
    _fontColor = color;
    _fontSize = fontSize;

    clear();

    return true;
}

void TextConsole::clear()
{
    _xPos = 0;
    _yPos = 0;
}

int TextConsole::gprintf(RenderTarget &target, const char *format, ...)
{
    char buffer[256];
    va_list args;
    va_start (args, format);
    vsprintf (buffer, format, args);
    va_end (args);

    /* TODO: add support for new lines and to wrap text around the
     * end of the render target */
    _fontRenderer->renderText(_xPos, _yPos, buffer, _fontColor, target);

    _yPos += _fontSize+4; // TODO: grab this information from the font renderer

    return 0;
}
