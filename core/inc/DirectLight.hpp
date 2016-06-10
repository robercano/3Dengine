/**
 * @class	DirectLight
 * @brief   This type of light is defined by a direction in space and has no cutoff point
 *          or attenuation, affects all things equally.
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include <glm/gtx/rotate_vector.hpp>
#include "Light.hpp"

class DirectLight : public Light
{
  public:
    DirectLight(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular, const glm::vec3 &direction)
        : Light(ambient, diffuse, specular)
    {
        glm::vec3 up(0.0f, 1.0f, 0.0f);

        /* Set the rotation according to the direction */
        setOrientation(glm::orientation(glm::normalize(direction), up));
    }
    ~DirectLight() {}
    const glm::mat4 &getProjectionMatrix() { return getOrthogonalMatrix(); }
};
