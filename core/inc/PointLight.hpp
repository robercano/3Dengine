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
//		PointLight(float attenuation = 1.0f) { _attenuation = attenuation; }
		PointLight(const glm::vec3 &ambient  = glm::vec3(0.0f, 0.0f, 0.0f),
			       const glm::vec3 &diffuse  = glm::vec3(0.0f, 0.0f, 0.0f),
			       const glm::vec3 &specular = glm::vec3(0.0f, 0.0f, 0.0f),
			       const glm::vec3 &position = glm::vec3(0.0f, 0.0f, 0.0f),
			       float attenuation = -1.0f,
			       float cutoff = -1.0f,
			       float coneAngle = 360.0f) :
			Light(ambient, diffuse, specular, position) {
        }
		~PointLight() { }
};
