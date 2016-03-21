/**
 * @class	NOAARenderTarget
 * @brief	A render target allows to render objects to it instead of
 *          to the main screen. The target can then be blitted onto the
 *          screen
 *
 *          The NOAA render target applies no anti-aliasing
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include "RenderTarget.hpp"

class NOAARenderTarget : public RenderTarget
{
	public:
        static NOAARenderTarget *New();
        static void Delete(NOAARenderTarget *target);

        virtual ~NOAARenderTarget() {}

        virtual bool init(uint32_t width, uint32_t height) = 0;
        virtual void bind() = 0;
        virtual void unbind() = 0;
        virtual void clear() = 0;
        virtual bool blit(uint32_t dstX, uint32_t dstY, uint32_t width, uint32_t height) = 0;
};
