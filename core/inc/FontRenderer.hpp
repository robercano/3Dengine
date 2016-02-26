/**
 * @class FontRenderer
 * @brief Uses a TrueType font to render text on a RenderTarget
 *
 * @author Roberto Cano
 */
#pragma once

#include <string>
#include <stdint.h>
#include "TrueTypeFont.hpp"
#include "RenderTarget.hpp"

class FontRenderer
{
    public:
        /* Singleton */
        static FontRenderer *GetFontRenderer();

        virtual bool setFont(TrueTypeFont *font) = 0;
        virtual bool renderText(std::string &text, RenderTarget &target) = 0;

	private:
		/**
		 * Singleton instance
		 */
		static FontRenderer *_fontRenderer;
};
