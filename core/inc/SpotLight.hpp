/**
 * @class	SpotLight
 * @brief   This type of light is defined by a point in space and a direction and it
 *          creates a cone of light, with a certain cone angle. Outside of the light cone
 *          the light has no effect
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include <glm/glm.hpp>
#include "Light.hpp"

class SpotLight : public Light
{
	public:
		SpotLight(const glm::vec3 &ambient = glm::vec3(1.0f, 1.0f, 1.0f),
			       const glm::vec3 &diffuse = glm::vec3(1.0f, 1.0f, 1.0f),
			       const glm::vec3 &specular = glm::vec3(1.0f, 1.0f, 1.0f),
			       const glm::vec3 &position = glm::vec3(0.0f, 0.0f, 0.0f),
				   float coneAngle = 45.0f,
				   float conePenumbra = 3.0f,
				   float attenuation = 1.0f,
				   float cutoff = 1e6f) :
			Light(ambient, diffuse, specular, position),
			_coneAngle(coneAngle),
			_conePenumbra(conePenumbra),
			_attenuation(attenuation),
			_cutoff(cutoff)
		{
        }
		~SpotLight() { }

		float getConeAngle() { return _coneAngle; }
		float getConePenumbra() { return _conePenumbra; }
		float getAttenuation() { return _attenuation; }
		float getCutoff() { return _cutoff; }

		void setConeAngle(float coneAngle) { _coneAngle = coneAngle; }
		void setConePenumbra(float conePenumbra) { _conePenumbra = conePenumbra; }
		void setAttenuation(float attenuation) { _attenuation = attenuation; }
		void setCutoff(float cutoff) { _cutoff = cutoff; }

		const glm::mat4 &getProjectionMatrix() { return getPerspectiveMatrix(); }

	protected:
		float     _coneAngle;
		float     _conePenumbra;
		float     _attenuation;
		float     _cutoff;
};
