/**
 * @class	PointLight
 * @brief   This type of light is defined by a point in space and radiates in a spherical
 *          manner. It can have attenuation.
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include <glm/glm.hpp>
#include "Light.hpp"

class PointLight : public Light
{
	public:
		PointLight(const glm::vec3 &ambient = glm::vec3(1.0f, 1.0f, 1.0f),
			       const glm::vec3 &diffuse = glm::vec3(1.0f, 1.0f, 1.0f),
			       const glm::vec3 &specular = glm::vec3(1.0f, 1.0f, 1.0f),
			       const glm::vec3 &position = glm::vec3(0.0f, 0.0f, 0.0f),
				   float attenuation = 1.0f,
				   float cutoff = 1e6f) :
			Light(ambient, diffuse, specular, position),
			_attenuation(attenuation),
			_cutoff(cutoff)
		{
        }
		~PointLight() { }

		float getAttenuation() { return _attenuation; }
		float getCutoff() { return _cutoff; }
		void setAttenuation(float attenuation) { _attenuation = attenuation; }
		void setCutoff(float cutoff) { _cutoff = cutoff; }

	protected:
		float _attenuation;
		float _cutoff;
};
