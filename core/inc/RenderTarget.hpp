/**
 * @class	RenderTarget
 * @brief	Interface for a render target. A render target allows to render objects to it
 *          instead of to the main screen. Then the target can be rendered to the main screen as
 *          a texture
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include <stdint.h>

class RenderTarget
{
	public:
        virtual void bind() = 0;
        virtual void unbind() = 0;
        virtual bool blit(uint32_t dstX, uint32_t dstY, uint32_t width, uint32_t height) = 0;
        virtual bool blit() { return blit(0, 0, _width, _height); }
        virtual void clear(float r, float g, float b, float a) = 0;
        virtual uint32_t getWidth()  { return _width; }
        virtual uint32_t getHeight() { return _height; }
    protected:
        uint32_t _width;
        uint32_t _height;
};
