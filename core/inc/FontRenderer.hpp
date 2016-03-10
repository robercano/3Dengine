/**
 * @class FontRenderer
 * @brief Uses a TrueType font to render text on a RenderTarget
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include <string>
#include <stdint.h>
#include <glm/glm.hpp>
#include "TrueTypeFont.hpp"
#include "RenderTarget.hpp"

class FontRenderer
{
    public:
        /* Singleton */
        static FontRenderer *New();
        static void Delete(FontRenderer *fontRenderer);

        virtual ~FontRenderer() {}

        virtual bool setFont(TrueTypeFont *font) = 0;
        virtual bool renderText(uint32_t x, uint32_t y, std::string &text, glm::vec4 &color, RenderTarget &target) = 0;
        virtual bool renderText(uint32_t x, uint32_t y, const char *text, glm::vec4 &color, RenderTarget &target) = 0;

	private:
		/**
		 * Singleton instance
		 */
		static FontRenderer *_fontRenderer;
};
