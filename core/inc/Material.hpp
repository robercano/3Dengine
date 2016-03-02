/**
 * @class	Material
 * @brief	Class to abstract a material. A material
 *          contains ambient, diffuse, specular, alpha and shininess
 *          components
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include <glm/glm.hpp>

class Material
{
	public:
        virtual ~Material() {}
        virtual bool setValues(glm::vec3 &ambient, glm::vec3 &diffuse, glm::vec3 &specular, float alpha, float shininess) = 0;
};
