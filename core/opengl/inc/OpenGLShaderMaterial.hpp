/**
 * @class	OpenGLShaderMaterial
 * @brief	OpenGL material implemented as a block uniform to be used in
 *          a shader. Contains ambient, diffuse, specular, alpha and shininess
 *          components
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include "OpenGL.h"
#include "OpenGLUniformBlock.hpp"
#include "Material.hpp"

class OpenGLShaderMaterial : public OpenGLUniformBlock
{
	public:
        void init(uint32_t bindingPoint);
        void copyMaterial(Material &material);
};
