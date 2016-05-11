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
		RenderTarget() : _r(0.0), _g(0.0), _b(0.0), _a(1.0) {}

		virtual bool init(uint32_t width, uint32_t height, uint32_t maxSamples = 0) = 0;
        virtual void bind() = 0;
		virtual void bindDepth() = 0;
        virtual void unbind() = 0;
        virtual bool blit(uint32_t dstX, uint32_t dstY, uint32_t width, uint32_t height, bool bindMainFB = true) = 0;
        virtual bool blit() { return blit(0, 0, _width, _height); }
        virtual void clear() = 0;

        virtual void setClearColor(float r, float g, float b, float a)
		{
			_r = r;
			_g = g;
			_b = b;
			_a = a;
		}

        virtual uint32_t getWidth()  { return _width; }
        virtual uint32_t getHeight() { return _height; }
    protected:
        uint32_t _width;
        uint32_t _height;
		float _r, _g, _b, _a;
};
