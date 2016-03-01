/**
 * @class	ShaderMaterial
 * @brief	Class to abstract a shader material. A material
 *          contains ambient, diffuse, specular, alpha and shininess
 *          components
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include <glm/glm.hpp>

class ShaderMaterial
{
	public:
        virtual init(glm::vec3 &ambient,
                     glm::vec3 &diffuse,
                     glm::vec3 &specular,
                     float     alpha,
                     float     shininess)
        {
            _ambient = ambient;
            _diffuse = diffuse;
            _specular = specular;
            _alpha = alpha;
            _shininess = shininess;
        }
        virtual ~ShaderMaterial() {}

    private:
        glm::vec3 _ambient;
        glm::vec3 _diffuse;
        glm::vec3 _specular;
        float     _alpha;
        float     _shininess;
};
