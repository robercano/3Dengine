/**
 * @class	OpenGLMaterial
 * @brief	OpenGL material implemented as a block uniform to be used in
 *          a shader. Contains ambient, diffuse, specular, alpha and shininess
 *          components
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include "Material.hpp"
#include "OpenGL.h"
#include "OpenGLUniformBlock.hpp"

class OpenGLMaterial : public OpenGLUniformBlock, public Material
{
	public:
        OpenGLMaterial();
        bool setValues(glm::vec3 &ambient, glm::vec3 &diffuse, glm::vec3 &specular, float alpha, float shininess);
};
