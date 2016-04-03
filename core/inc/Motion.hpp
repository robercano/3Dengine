/**
 * @class	Motion
 * @brief	Motion object. Applies certain type of motion to an Object3D
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include "Object3D.hpp"

class Motion
{
	public:
		virtual void applyTo(Object3D &obj) = 0;
		virtual void reset() = 0;
};
