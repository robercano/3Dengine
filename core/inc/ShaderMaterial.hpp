/**
 * @class	ShaderMaterial
 * @brief	Class to access a shader material
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include "Material.hpp"

class ShaderMaterial
{
	public:
        virtual ~ShaderMaterial() {}
        virtual bool copyMaterial(Material &material) = 0;
};
