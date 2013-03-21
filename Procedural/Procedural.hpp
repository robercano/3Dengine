/**
 * @module  Procedural
 * @brief	Procedural objects generation library
 *
 * @author	Roberto Sosa Cano
 */
#ifndef __PROCEDURAL_HPP__
#define __PROCEDURAL_HPP__

#include <stdint.h>
#include <vector>

namespace procedural
{
	/**
	 * Internally used structures
	 */
	struct vertex {
		float x, y, z, w;
	};

	struct color {
		float r, g, b, a;
	};

	struct normal {
		float x, y, z, w;
	};

	struct pack {
		vertex v;
		color  c;
		normal n;
	} __attribute((packed));

	enum Axis {
		AXIS_X,
		AXIS_Y,
		AXIS_Z
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
			 Plane(uint32_t horizontal, uint32_t vertical, float width = 1.0, float height = 1.0,
					Axis axis = AXIS_X, float shift = 0.0);

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
			  * Array of data containing the plane geometry
			  */
			 std::vector<pack> _packs;

			 /**
			  * Vector of indices in triangle strip format
			  */
			 std::vector<uint32_t> _indices;

			 /**
			  * Dimensions of the plane
			  */
			 uint32_t _width;
			 uint32_t _height;

			 /**
			  * Number of horizontal and vertical vertices
			  */
			 uint32_t _horizontal;
			 uint32_t _vertical;
	};
};

#endif
