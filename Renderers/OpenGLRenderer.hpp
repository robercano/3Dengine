/**
 * @class	OpenGLRenderer
 * @brief	OpenGL based 3D renderer
 *
 * @author	Roberto Sosa Cano
 */
#ifndef __OPENGLRENDERER_HPP__
#define __OPENGLRENDERER_HPP__

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
		 * Initializes the renderer
		 */
		void init(void);

		/**
		 * OpenGL shader factory
		 */
		Shader * getShader(void);

		/**
		 * Updates the display
		 *
		 * @param projection	Projection matrix
		 * @param view			View matrix
		 */
		bool render(const glm::mat4 &projection, const glm::mat4 &view);

		/**
		 * Adjusts the display size
		 *
		 * @param	width	New width of the display
		 * @param	height	New height of the display
		 *
		 * @return  true or false
		 */
		bool resize(uint16_t width, uint16_t height);

		/**
		 * Adds a new 3D object to be rendered
		 */
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

#endif
