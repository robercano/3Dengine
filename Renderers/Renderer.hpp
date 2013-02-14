/**
 * @class	Renderer
 * @brief	Interface for software/hardware renderers
 *
 * @author	Roberto Sosa Cano
 */

#ifndef __RENDERER_HPP__
#define __RENDERER_HPP__

#include "Object3D.hpp"

class Renderer
{
	public:
		/**
		 * Destructor
		 */
		virtual ~Renderer() {}

		/**
		 * Initializes the renderer
		 */
		virtual void init(void) = 0;

		/**
		 * Updates the display contents
		 */
		virtual bool render() = 0;

		/**
		 * Adjusts the renderer's display size
		 */
		virtual bool resize(uint16_t width, uint16_t height) = 0;

		/**
		 * Adds a new 3D object to be rendered
		 */
		virtual bool addObject(Object3D *object) = 0;
};

#endif
