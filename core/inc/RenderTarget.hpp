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
        /**
         * Initializes the target with the given size
         *
         * @param width  Width of the render target
         * @param height Height of the render target
         */
        virtual bool init(uint32_t width, uint32_t height) = 0;

       /**
        * Returns the render target ID that will be usedby the renderer
        * to render the objects into the target
        *
        */
        virtual const uint32_t getID() = 0;

        /**
         * Renders the target to screen
         */
        virtual bool render() = 0;
};
