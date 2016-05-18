/**
 * @class	PointLight
 * @brief   This type of light is defined by a point in space and radiates in a spherical
 *          manner. It can have attenuation.
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include "Light.hpp"

class PointLight : public Light
{
	public:
		PointLight(const glm::vec3 &ambient,
			       const glm::vec3 &diffuse,
			       const glm::vec3 &specular,
			       const glm::vec3 &position) :
			Light(ambient, diffuse, specular, position) {
        }
		~PointLight() { }
};
