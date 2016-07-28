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
#include <stdint.h>

class Material
{
  public:
    friend class ModelStorage;

    /**
     * This constant defines the expected size of the Material structure
     * if it is correctly packed. It is used to optimize the serialization
     * process of the model's material data
     */
    static const uint32_t PackedSize = 44;

    /* The default constructor sets some wierd material
     * to identify it visually if some materials are not
     * set properly */
    Material(bool strange = false)
        : _ambient(glm::vec3(0.2f, 0.2f, 0.2f))
        , _diffuse(glm::vec3(1.0f, 1.0f, 1.0f))
        , _specular(glm::vec3(0.0f, 0.0f, 0.0f))
        , _alpha(1.0)
        , _shininess(0.0f)
    {
        if (strange) {
            _ambient = glm::vec3(1.0f, 1.0f, 0.0f);
            _diffuse = glm::vec3(0.0f, 1.0f, 1.0f);
            _specular = glm::vec3(1.0f, 0.0f, 1.0f);
            _alpha = 1.0;
            _shininess = 0.5f;
        }
    }

    Material(glm::vec3 &ambient, glm::vec3 &diffuse, glm::vec3 &specular, float alpha, float shininess)
        : _ambient(ambient), _diffuse(diffuse), _specular(specular), _alpha(alpha), _shininess(shininess)
    {
    }

    void setAmbient(glm::vec3 &ambient) { _ambient = ambient; }
    void setDiffuse(glm::vec3 &diffuse) { _diffuse = diffuse; }
    void setSpecular(glm::vec3 &specular) { _specular = specular; }
    void setAlpha(float alpha) { _alpha = alpha; }
    void setShininess(float shininess) { _shininess = shininess; }
    glm::vec3 getAmbient() const { return _ambient; }
    glm::vec3 getDiffuse() const { return _diffuse; }
    glm::vec3 getSpecular() const { return _specular; }
    float getAlpha() const { return _alpha; }
    float getShininess() const { return _shininess; }
  private:
    glm::vec3 _ambient;
    glm::vec3 _diffuse;
    glm::vec3 _specular;
    float _alpha;
    float _shininess;
};
