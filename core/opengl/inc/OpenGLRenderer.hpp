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
		 * Destructor
		 */
		~OpenGLRenderer();

		/**
		 * Renderer methods
		 */
		void init(void);
		Shader * getShader(void);
		bool render(const glm::mat4 &projection, const glm::mat4 &view, RenderTarget &renderTarget);
		bool resize(uint16_t width, uint16_t height);
		bool addObject(Object3D *object);

	private:
		/**
		 * Width of the display
		 */
		uint16_t _width;

		/**
		 * Height of the display
		 */
		uint16_t _height;

		/**
		 * List of 3D objects to be rendered
		 */
		std::vector<Object3D*> _objects;
};
