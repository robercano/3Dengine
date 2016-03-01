/**
 * @class	OpenGLRenderer
 * @brief	OpenGL based 3D renderer
 *
 * @author	Roberto Sosa Cano
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
        RendererObject3D *prepareObject3D(const Object3D &object);
		bool renderObject3D(RendererObject3D &object, Shader &shader,
                            const glm::mat4 &projection, const glm::mat4 &view,
                            RenderTarget &renderTarget);
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
