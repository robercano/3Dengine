/**
 * @class	Light
 * @brief   Light object
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class Light
{
	public:
		Light() {}
		Light(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular) :
            _ambient(ambient), _diffuse(diffuse), _specular(specular), _position(glm::vec3(0.0)) {
        }
		Light(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular, const glm::vec3 &position) :
            _ambient(ambient), _diffuse(diffuse), _specular(specular), _position(position) {}
		~Light() {}

		void setPosition(const glm::vec3 &position) { _position = position; }
        void setAmbient(const glm::vec3 &ambient) { _ambient = ambient; }
        void setDiffuse(const glm::vec3 &diffuse) { _diffuse = diffuse; }
        void setSpecular(const glm::vec3 &specular) { _specular = specular; }

        glm::vec3 getPosition()  { return _position; }
        glm::vec3 getAmbient()  { return _ambient; }
        glm::vec3 getDiffuse()  { return _diffuse; }
        glm::vec3 getSpecular() { return _specular; }

	protected:
		glm::vec3 _position;
        glm::vec3 _ambient;
        glm::vec3 _diffuse;
        glm::vec3 _specular;
};
