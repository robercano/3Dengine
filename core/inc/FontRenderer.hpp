/**
 * @class FontRenderer
 * @brief Uses a TrueType font to render text on a RenderTarget. This is the
 *        high level API for the font renderer which is then specialized
 *        depending on the rendering API
 *
 *        It allows to set a specific TrueType font for rendering and then
 *        render the desired text onto a render target with the given color
 *
 *        This class does NOT support formatting like printf. For that functionality
 *        instead use TextConsole class
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include <stdint.h>
#include <glm/glm.hpp>
#include <string>
#include "RenderTarget.hpp"
#include "TrueTypeFont.hpp"

class FontRenderer
{
  public:
    /**
     * Allocates a new FontRenderer of the specific underlaying API
     *
     * To free the returned memory FontRenderer::Delete() must be used
     *
     * @return A pointer to the newly allocated FontRenderer
     */
    static FontRenderer *New();

    /**
     * Frees the shader previously allocated by FontRenderer::New()
     *
     * @param target  Pointer to the allocated FontRenderer
     */
    static void Delete(FontRenderer *fontRenderer);

    /**
     * Destructor
     */
    virtual ~FontRenderer() {}

    /**
     * Sets the new font to be used for rendering
     *
     * @param font  The new font to be used for rendering
     *
     * @return true if the font was set correctly or false otherwise
     */
    virtual bool setFont(TrueTypeFont *font) = 0;

    /**
     * Renders the given text onto the given render target with the indicated color
     * at the indicated (x, y) position on the render target
     *
     * @param x      X coordinate on the render target to render the text at
     * @param y      Y coordinate on the render target to render the text at
     * @param text   Text to be rendered
     * @param color  Color to used for the rendering as normalized (0.0-1.0) RGBA
     * @param target Render target to render the text on
     *
     * @return true if the text was rendered correctly or false otherwise
     */
    virtual bool renderText(uint32_t x, uint32_t y, std::string &text, glm::vec4 &color, RenderTarget &target) = 0;
    virtual bool renderText(uint32_t x, uint32_t y, const char *text, glm::vec4 &color, RenderTarget &target) = 0;
};
