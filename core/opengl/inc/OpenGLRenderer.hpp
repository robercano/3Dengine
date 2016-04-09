/**
 *                          Light &light,
 * @class	OpenGLRenderer
 * @brief	OpenGL based 3D renderer
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include <vector>
#include "Renderer.hpp"

class OpenGLRenderer : public Renderer
{
	public:
		/**
		 * Renderer methods
		 */
		void init(void);
		Shader * newShader(void);
        RendererModel3D *prepareModel3D(const Model3D &model);
		bool renderModel3D(RendererModel3D &model, Camera &camera,
                           LightingShader &shader, std::vector<Light*> &lights, float ambientK,
                           RenderTarget &renderTarget, bool disableDepth = false);
		bool resize(uint16_t width, uint16_t height);
        void flush();

	private:
		/**
		 * Width of the display
		 */
		uint16_t _width;

		/**
		 * Height of the display
		 */
		uint16_t _height;
};
