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
        /* The default constructor sets some wierd material
         * to identify it visually if some materials are not
         * set properly */
        Material(bool strange=false) :
            _ambient(glm::vec3(0.2f, 0.2f, 0.2f)),
            _diffuse(glm::vec3(1.0f, 1.0f, 1.0f)),
            _specular(glm::vec3(0.0f, 0.0f, 0.0f)),
            _alpha(1.0),
            _shininess(0.0f)
        {
            if (strange) {
                _ambient  = glm::vec3(0.0f, 1.0f, 0.0f);
                _diffuse  = glm::vec3(0.0f, 1.0f, 0.0f);
                _specular = glm::vec3(0.0f, 1.0f, 0.0f);
                _alpha = 1.0;
                _shininess = 0.5f;
            }
        }

        Material(glm::vec3 &ambient, glm::vec3 &diffuse, glm::vec3 &specular, float alpha, float shininess) :
            _ambient(ambient),
            _diffuse(diffuse),
            _specular(specular),
            _alpha(alpha),
            _shininess(shininess) {}

        void setAmbient(glm::vec3 &ambient) { _ambient = ambient; }
        void setDiffuse(glm::vec3 &diffuse) { _diffuse = diffuse; }
        void setSpecular(glm::vec3 &specular) { _specular = specular; }
        void setAlpha(float alpha) { _alpha = alpha; }
        void setShininess(float shininess) { _shininess = shininess; }

        glm::vec3 getAmbient()  { return _ambient; }
        glm::vec3 getDiffuse()  { return _diffuse; }
        glm::vec3 getSpecular() { return _specular; }
        float     getAlpha()    { return _alpha; }
        float     getShininess(){ return _shininess; }
    private:
        glm::vec3 _ambient;
        glm::vec3 _diffuse;
        glm::vec3 _specular;
        float     _alpha;
        float     _shininess;
};
