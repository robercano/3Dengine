/**
 * @class	TextConsole
 * @brief	Implements an on-screen text console. It provides printf-like capabilities
 *          to print text to a render target. It allows to choose the color, the font type
 *          and the font size.
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#include <stdarg.h>
#include <stdio.h>

#include "NOAARenderTarget.hpp"
#include "TextConsole.hpp"

#define FONT_INTERLINE_DISTANCE 4
#define SCREEN_TOP_MARGIN 5
#define SCREEN_LEFT_MARGIN 5

bool TextConsole::init(std::string &fontPath, uint32_t fontSize, uint32_t width, uint32_t height)
{
    NOAARenderTarget *target = NOAARenderTarget::New();
    if (target == NULL) {
        return false;
    }

    if (target->init(width, height) != true) {
        NOAARenderTarget::Delete(target);
        return false;
    }

    /* Setup the font renderer */
    _font = TrueTypeFont::New();

    if (_font->init(fontPath, fontSize) == false) {
        delete _font;
        NOAARenderTarget::Delete(target);
        return false;
    }

    _fontRenderer = FontRenderer::New();
    if (_fontRenderer == NULL) {
        delete _font;
        NOAARenderTarget::Delete(target);
        return false;
    }

    _renderTarget = target;
    _fontRenderer->setFont(_font);
    _fontSize = fontSize;

    _foreground = glm::vec4(1.0);
    _background = glm::vec4(0.0);

    clear();

    return true;
}

void TextConsole::setForegroundColor(float r, float g, float b, float a) { _foreground = glm::vec4(r, g, b, a); }
void TextConsole::setBackgroundColor(float r, float g, float b, float a)
{
    _background = glm::vec4(r, g, b, a);
    _renderTarget->setClearColor(_background.r, _background.g, _background.b, _background.a);
}

void TextConsole::clear()
{
    _xPos = SCREEN_TOP_MARGIN;
    _yPos = SCREEN_LEFT_MARGIN;

    _renderTarget->clear();
}

int TextConsole::gprintf(const char *format, ...)
{
    char buffer[256];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof buffer, format, args);
    va_end(args);

    char line[256];
    uint32_t linePos = 0;
    char tab[] = "    ";
    uint32_t xSize = _xPos, i;

    for (i = 0; buffer[i] != '\0'; ++i) {
        /* Parse the lines */
        uint32_t dummy, advance;
        _font->getBitmap(buffer[i], dummy, dummy, dummy, dummy, advance);

        if (buffer[i] == '\t') {
            line[linePos] = '\0';
            _fontRenderer->renderText(_xPos, _yPos, line, _foreground, *_renderTarget);
            linePos = 0;

            _font->getBitmap(' ', dummy, dummy, dummy, dummy, advance);
            xSize += (sizeof tab - 1) * advance;
            _xPos = xSize;
        }
        if (buffer[i] == '\n' || (xSize + advance) > _renderTarget->getWidth()) {
            line[linePos] = '\0';
            /* Weird formula to adjust interline space.
             * Just to make it around 4 pixels for a 14 pixels
             * font */
            _fontRenderer->renderText(_xPos, _yPos, line, _foreground, *_renderTarget);
            _yPos += 4 * _fontSize / 3;
            linePos = 0;
            xSize = SCREEN_LEFT_MARGIN;
            _xPos = SCREEN_LEFT_MARGIN;
        }
        if (buffer[i] != '\n' && buffer[i] != '\t') {
            line[linePos++] = buffer[i];
            xSize += advance;
        }
    }
    if (linePos != 0) {
        line[linePos] = '\0';
        _fontRenderer->renderText(_xPos, _yPos, line, _foreground, *_renderTarget);
        linePos = 0;
        _xPos = xSize;
    }
    return 0;
}

void TextConsole::blit() { _renderTarget->blit(); }
