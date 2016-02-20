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
#include "RendererObject3D.hpp"
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
         * Wraps an object3D with a renderer specific object 3D,
         * peparing all the structures needed to quickly render
         * the object in the renderer
         *
         * The returned object can be deleted through a delete
         * statement. The input Object3D has no linkage with the
         * output RenderObject3D and hence they can have different
         * lif cycles
         *
         * @param object  Object to prepare in the renderer and used to
         *                generate the RendererObject3D
         *
         * @return The prepared RendererObject3D pointer or NULL if an
         *         error happened
         */
        virtual RendererObject3D *prepareObject3D(const Object3D &object) = 0;

		/**
		 * Renders the internal list of objects to a render target
		 *
		 * @param projection	Projection matrix
		 * @param view			View matrix
		 */
		virtual bool renderObject3D(RendererObject3D &object, Shader &shader,
                                    const glm::mat4 &projection, const glm::mat4 &view,
                                    RenderTarget &renderTarget) = 0;

		/**
		 * Adjusts the renderer's display size
		 */
		virtual bool resize(uint16_t width, uint16_t height) = 0;
	private:
		/**
		 * Singleton instance
		 */
		static Renderer *_renderer;
};
