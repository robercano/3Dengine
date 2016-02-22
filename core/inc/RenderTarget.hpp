/**
 * @class	RenderTarget
 * @brief	Interface for a render target. A render target allows to render objects to it
 *          instead of to the main screen. Then the target can be rendered to the main screen as
 *          a texture
 *
 * @author	Roberto Sosa Cano (http://www.robertocano.es)
 */
#pragma once

class RenderTarget
{
	public:
        virtual void bind() = 0;
        virtual void unbind() = 0;
        virtual bool blit(uint32_t dstX, uint32_t dstY, uint32_t width, uint32_t height) = 0;
};
