/**
 * @module  Procedural
 * @brief	Procedural objects generation library
 *
 * @author	Roberto Sosa Cano
 */
#include "Procedural.hpp"

using namespace procedural;
using namespace std;

Plane::Plane(uint32_t horizontal, uint32_t vertical, float width, float height,
		     Axis axis, float shift)
{
	_horizontal = horizontal;
	_vertical   = vertical;
	_width      = width;
	_height     = height;

	/*
	 * For each row of vertices of the plane two indices are needed
	 * per column, plus one at the beggining and other at the end
	 * for degenerate triangles. That is number of 2*columns + 2.
	 * Then multiplied by the number of rows minus one, as the last
	 * row does have to generate more triangles
	 */
	_packs.resize(_horizontal*_vertical);
	_indices.resize(2*(_horizontal+1)*(_vertical-1)-2);

	float hDiv = _horizontal - 1;
	float vDiv = _vertical   - 1;

	pack *data = &_packs[0];

	/* Precalculate the normal */
	normal X_AXIS = { 1.0, 0.0, 0.0, 0.0 },
		   Y_AXIS = { 0.0, 1.0, 0.0, 0.0 },
		   Z_AXIS = { 0.0, 0.0, 1.0, 0.0 };
	normal xaxis, yaxis, zaxis;

	switch (axis) {
		case AXIS_X:
			xaxis = X_AXIS;
			yaxis = Y_AXIS;
			zaxis = Z_AXIS;
			break;
		case AXIS_Y:
			xaxis = Y_AXIS;
			yaxis = X_AXIS;
			zaxis = Z_AXIS;
			break;
		case AXIS_Z:
			xaxis = Z_AXIS;
			yaxis = Y_AXIS;
			zaxis = X_AXIS;
			break;
	}

	/* Generate the plane vertices */
	uint32_t count = 0;
	for (int i=0; i<_vertical; ++i) {
		for (int j=0; j<_horizontal; ++j) {
			data[count].v.x = shift*xaxis.x + xaxis.y*i/vDiv + xaxis.z*j/hDiv;
			data[count].v.y = shift*yaxis.x + yaxis.y*i/vDiv + yaxis.z*j/hDiv;
			data[count].v.z = shift*zaxis.x + zaxis.y*i/vDiv + zaxis.z*j/hDiv;
			data[count].v.w = 1.0;

			data[count].n = xaxis;

			data[count].c.r = data[count].v.x; //1.0;
			data[count].c.g = data[count].v.y; //1.0;
			data[count].c.b = data[count].v.z; //1.0;
			data[count].c.a = 1.0;

			count++;
		}
	}

	/* Generate the indices */
	uint32_t *index = &_indices[0];

	count = 0;
	for (int i=0; i<_vertical-1; ++i) {
		/* Repeat the first index */
		if (i != 0) {
			index[count++] = _horizontal*(i+0) + 0;
		}
		for (int j=0; j<_horizontal; ++j) {
			index[count++] = _horizontal*(i+0) + j;
			index[count++] = _horizontal*(i+1) + j;
		}
		/* Repeat the last index */
		if (i != (_vertical-2)) {
			index[count++] = _horizontal*(i+1) + _horizontal-1;
		}
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
