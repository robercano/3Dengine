/**
 * @module  Plane
 * @brief	Plane procedural generation
 *
 * @author	Roberto Sosa Cano
 */
#pragma once

#include <stdint.h>
#include <vector>
#include <glm/glm.hpp>

namespace procedural
{
	/**
	 * Vertex data structure
	 */
	struct pack {
		glm::vec4 vertex;
		glm::vec4 color;
		glm::vec4 normal;
	};

	/**
	 * Plane class to generate axis aligned planes
	 */
	class Plane
	{
		public:
			/**
			 * Generates a plane of size width by height and
			 * using horizontal by vertical vertices.
			 * The function generates all the vertices, colors,
			 * normals and indices for a triangle strip geometry
			 *
			 * @details	The plane will be centered on two of
			 * the axis and will the indicated axis at the
			 * indicated position
			 *
			 * @param width      Width of the plane
			 * @param height     Height of the plane
			 * @param horizontal Number of vertices along width
			 * @param vertical   Number of vertices along height
			 * @param axis       Axis that defines the plane
			 * @param shift      Shift along the indicated axis
			 *
			 * @return A plane object that contains the geometry
			 */
			 Plane(void);
			 void addPlane(uint32_t horizontal = 2, uint32_t vertical = 2,
					       float xscale = 1.0, float yscale = 1.0, float zscale = 1.0,
						   glm::vec4 &xaxis = XAXIS,
						   glm::vec4 &yaxis = YAXIS,
						   glm::vec4 &zaxis = ZAXIS);

			 /**
			  * Returns the internal geometry data
			  */
			 const std::vector<pack> * getGeometry(void);

			 /**
			  * Returns the internal indices for the geometry
			  */
			 const std::vector<uint32_t> * getIndices(void);

		private:
			 /**
			  * Default axis for plane generation
			  */
			 static glm::vec4 XAXIS;
			 static glm::vec4 YAXIS;
			 static glm::vec4 ZAXIS;

			 /**
			  * Array of data containing the plane geometry
			  */
			 std::vector<pack> _packs;

			 /**
			  * Vector of indices in triangle strip format
			  */
			 std::vector<uint32_t> _indices;
	};
};
