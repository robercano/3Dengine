/**
 * @module  Procedural
 * @brief	Procedural objects generation library
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#include "Plane.hpp"
#include <glm/glm.hpp>

using namespace procedural;
using namespace std;

glm::vec4 Plane::XAXIS(1.0, 0.0, 0.0, 0.0);
glm::vec4 Plane::YAXIS(0.0, 1.0, 0.0, 0.0);
glm::vec4 Plane::ZAXIS(0.0, 0.0, 1.0, 0.0);

Plane::Plane()
{
}

void Plane::addPlane(uint32_t horizontal, uint32_t vertical,
					 float xscale, float yscale, float zscale,
					 glm::vec4 &xaxis, glm::vec4 &yaxis, glm::vec4 &zaxis)
{
	/* Scale vectors */
	xaxis *= xscale;
	yaxis *= yscale;
	zaxis *= zscale;

	glm::vec4 tmp = xaxis/2.0;
	float xcenter = tmp.x + tmp.y + tmp.z;

	tmp = yaxis/2.0;
	float ycenter = tmp.x + tmp.y + tmp.z;

	tmp = zaxis/2.0;
	float zcenter = tmp.x + tmp.y + tmp.z;

	/*
	 * For each row of vertices of the plane two indices are needed
	 * per column, plus one at the beggining and other at the end
	 * for degenerate triangles. That is number of 2*columns + 2.
	 * Then multiplied by the number of rows minus one, as the last
	 * row does have to generate more triangles
	 */
	uint32_t pack_size    = _packs.size();
	uint32_t indices_size = _indices.size();

	_packs.resize(pack_size + horizontal*vertical);
	if (pack_size == 0) {
		_indices.resize(indices_size + 2*(horizontal+1)*(vertical-1) - 1);
	} else {
		_indices.resize(indices_size + 2*(horizontal+1)*(vertical-1));
	}

	float hDiv = horizontal - 1;
	float vDiv = vertical   - 1;

	pack *data = &_packs[pack_size];

	/* Generate the plane vertices */
	uint32_t count = 0;
	for (int i=0; i<vertical; ++i) {
		for (int j=0; j<horizontal; ++j) {
			data[count].vertex.x = xaxis.x + xaxis.y*i/vDiv + xaxis.z*j/hDiv - xcenter;
			data[count].vertex.y = yaxis.x + yaxis.y*i/vDiv + yaxis.z*j/hDiv - ycenter;
			data[count].vertex.z = zaxis.x + zaxis.y*i/vDiv + zaxis.z*j/hDiv - zcenter;
			data[count].vertex.w = 1.0;

			data[count].normal = xaxis;

			data[count].color.r = data[count].vertex.x + 0.5;
			data[count].color.g = data[count].vertex.y + 0.5;
			data[count].color.b = data[count].vertex.z + 0.5;
			data[count].color.a = 1.0;
			count++;
		}
	}

	/* Generate the indices */
	uint32_t *index = &_indices[indices_size];
	count = 0;
	for (int i=0; i<vertical-1; ++i) {
		/* Always repeat the first index, in case more geometry must be added */
		if (pack_size != 0 || i!=0) {
			index[count++] = pack_size + horizontal*(i+0) + 0;
		}
		for (int j=0; j<horizontal; ++j) {
			index[count++] = pack_size + horizontal*(i+0) + j;
			index[count++] = pack_size + horizontal*(i+1) + j;
		}
		/* Same as the first index */
		index[count++] = pack_size + horizontal*(i+1) + horizontal-1;
	}
}

const vector<pack> * Plane::getGeometry(void)
{
	return &_packs;
}

const vector<uint32_t> * Plane::getIndices(void)
{
	return &_indices;
}
