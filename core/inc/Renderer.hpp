/**
 * @class	Renderer
 * @brief	Interface for software/hardware renderers
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include "Model3D.hpp"
#include "RenderTarget.hpp"
#include "RendererModel3D.hpp"
#include "Shader.hpp"
#include "Camera.hpp"

class Renderer
{
	public:
		/**
		 * Singleton
		 */
		static Renderer *GetInstance(void);
		static void DisposeInstance(void);

		/**
		 * Destructor
		 */
		virtual ~Renderer() {}

		/**
		 * Initializes the renderer
		 */
		virtual void init(void) = 0;

        /**
         * Wraps an model3D with a renderer specific model 3D,
         * peparing all the structures needed to quickly render
         * the model in the renderer
         *
         * The returned model can be deleted through a delete
         * statement. The input Model3D has no linkage with the
         * output RenderModel3D and hence they can have different
         * lif cycles
         *
         * @param model  Object to prepare in the renderer and used to
         *                generate the RendererModel3D
         *
         * @return The prepared RendererModel3D pointer or NULL if an
         *         error happened
         */
        virtual RendererModel3D *prepareModel3D(const Model3D &model) = 0;

		/**
		 * Renders the internal list of models to a render target
		 *
		 * @param projection	Projection matrix
		 * @param view			View matrix
		 */
		virtual bool renderModel3D(RendererModel3D &model, Camera &camera,
                                   Shader &shader, std::vector<Light*> &lights, float ambientK,
                                   RenderTarget &renderTarget) = 0;

		/**
		 * Adjusts the renderer's display size
		 */
		virtual bool resize(uint16_t width, uint16_t height) = 0;
        virtual void flush() = 0;
	private:
		/**
		 * Singleton instance
		 */
		static Renderer *_renderer;
};
