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
        /**
         * Constructor
         */
        RenderTarget() : _width(0), _height(0), _r(0.0), _g(0.0), _b(0.0), _a(1.0), _blendingMode(BLENDING_NONE) {}

        /**
         * Initializes the render target with the given configuration
         *
         * @width       Width of the render target
         * @height      Height of the render target
         * @maxSamples  Maximum number of samples for multi-sample targets like MSAA
         * @numTarget   Number of targets for multi-target render targets
         *
         * @return true or false
         */
        virtual bool init(uint32_t width, uint32_t height, uint32_t maxSamples = 0, uint32_t numTargets = 1) = 0;

        /**
         * Binds the render target to the current render context so it receives the
         * retsult of the rendering commands. It binds all buffers belonging to the
         * target (color, depth, stencil, etc...)
         */
        virtual void bind() = 0;

        /**
         * Similar to bind() but it binds only the depth buffer of the render target
         */
        virtual void bindDepth() = 0;

        /**
         * Unbinds the render target from the current rendering context
         */
        virtual void unbind() = 0;

        /**
         * Blits the contents of the render target onto the main frambuffer
         * or onto another render target.
         *
         * In case of a multi-target render target, one of the multi-targets must
         * be selected for the blitting operation using the 'target' parameter
         *
         * @param dstX       X coordinate in the destination buffer where the X origin
         *                   of the target being blitted will be copied to
         * @param dstY       Y coordinate in the destination buffer where the Y origin
         *                   of the target being blitted will be copied to
         * @param width      Width of the source target to copy onto the final buffer
         * @param height     Height of the source target to copy onto the final buffer
         * @param target     In a multi-target configuration, the number of the source
         *                   target to use for blitting, use 0 for a single target
         * @param bindMainFB If 'true' it binds the main framebuffer by defaults. Use this
         *                   to blit into the final buffer. To blit into another render
         *                   target you must pass 'false'
         *
         * @return true or false
         */
        virtual bool blit(uint32_t dstX, uint32_t dstY, uint32_t width, uint32_t height, uint32_t target = 0, bool bindMainFB = true) = 0;

        /**
         * Convenience function with defaults to blit the whole render target
         * into the main framebuffer
         */
        virtual bool blit() { return blit(0, 0, _width, _height); }

        /**
         * Clears the contents of all buffers in all targets associated
         * with this render target
         */
        virtual void clear() = 0;

        /**
         * Sets the clear color for all color buffers of all targets
         * associated with this render target
         */
        virtual void setClearColor(float r, float g, float b, float a)
        {
            _r = r;
            _g = g;
            _b = b;
            _a = a;
        }

        /**
         * Blending mode used in blitting operations
         */
        enum BlendingMode {
            BLENDING_NONE = 0,     /**> No blending mode */
            BLENDING_ADDITIVE = 1  /**> Additive blending mode */
        };

        /**
         * Sets the blending mode for the next blitting operation
         *
         * @mode  The new blending mode. @see BlendingMode
         */
        virtual void setBlendingMode(BlendingMode mode) { _blendingMode = mode; }

        /**
         * Gets the width and the height of the render target
         */
        virtual uint32_t getWidth() { return _width; }
        virtual uint32_t getHeight() { return _height; }

    protected:
        uint32_t _width;            /**> Width of the render target */
        uint32_t _height;           /**> Height of the render target */
        float _r, _g, _b, _a;       /**> RGBA components for the clear color */
        BlendingMode _blendingMode; /**> Blending mode for the blitting operation */
};
