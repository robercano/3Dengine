/**
 * @class	Renderer
 * @brief	Interface for software/hardware renderers
 *
 * @author	Roberto Sosa Cano
 */
#pragma once

#include <string>
#include <glm/glm.hpp>
#include "Object3D.hpp"
#include "RenderTarget.hpp"
#include "Shader.hpp"

class Renderer
{
	public:
		/**
		 * Singleton
		 */
		static Renderer *GetRenderer(void);

		/**
		 * Shader factory
		 */
		virtual Shader * getShader(void) = 0;

		/**
		 * Destructor
		 */
		virtual ~Renderer() {}

		/**
		 * Initializes the renderer
		 */
		virtual void init(void) = 0;

		/**
		 * Renders the internal list of objects to a render target
		 *
		 * @param projection	Projection matrix
		 * @param view			View matrix
		 */
		virtual bool render(const glm::mat4 &projection, const glm::mat4 &view, RenderTarget &renderTarget) = 0;

		/**
		 * Adjusts the renderer's display size
		 */
		virtual bool resize(uint16_t width, uint16_t height) = 0;

		/**
		 * Adds a new 3D object to be rendered
		 */
		virtual bool addObject(Object3D *object) = 0;

	private:
		/**
		 * Singleton instance
		 */
		static Renderer *_renderer;
};
