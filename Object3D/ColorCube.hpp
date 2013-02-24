/**
 * Cube object with RGB colors
 *
 * @author	Roberto Sosa Cano
 */
#ifndef __COLORCUBE_HPP__
#define __COLORCUBE_HPP__

#include <stdint.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Object3D.hpp"

class ColorCube : public Object3D
{
	public:
		/**
		 * Destructor
		 */
		~ColorCube();

		/**
		 * Initialisation method
		 *
		 * @return true or false
		 */
		bool init();

		/**
		 * Destroy the geometry
		 *
		 * @return true or false
		 */
		bool destroy();

		/**
		 * Render the object
		 *
		 * return true or false
		 */
		bool render();

private:
		glm::mat4 _MVP;
		uint32_t _VertexArrayID;
		uint32_t _MatrixID;
		uint32_t _vertexbuffer;
		uint32_t _colorbuffer;
		uint32_t _programID;
};

#endif
