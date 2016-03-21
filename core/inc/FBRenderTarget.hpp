/**
 * @class	FBRenderTarget
 * @brief	A render target allows to render objects to it instead of
 *          to the main screen. The target can then be blitted onto the
 *          screen
 *
 *          The FB render target applies no anti-aliasing
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include "RenderTarget.hpp"

class FBRenderTarget : public RenderTarget
{
	public:
        static FBRenderTarget *New();
        static void Delete(FBRenderTarget *target);

        virtual ~FBRenderTarget() {}

        virtual bool init(uint32_t width, uint32_t height) = 0;
        virtual void bind() = 0;
        virtual void unbind() = 0;
        virtual void clear() = 0;
        virtual bool blit(uint32_t dstX, uint32_t dstY, uint32_t width, uint32_t height) = 0;
};
