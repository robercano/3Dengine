/**
 * @class	ShaderLight
 * @brief	Class to access a shader light
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include "Light.hpp"

class ShaderLight
{
	public:
        virtual ~ShaderLight() {}
        virtual bool copyLight(Light &light) = 0;
};
