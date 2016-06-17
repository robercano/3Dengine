/**
 * @class	DirectLight
 * @brief   This type of light is defined by a direction in space and has no cutoff point
 *          or attenuation, affects all things equally. Typical use is for the light radiated
 *          from the sun or the moon
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include <glm/gtx/rotate_vector.hpp>
#include "Light.hpp"

class DirectLight : public Light
{
  public:
    /**
     * Constructor
     *
     * @param ambient  Ambient component of the light
     * @param diffuse  Diffuse component of the light
     * @param specular Specular component of the light
     * @param direction Direction of the light. It assumes w=0
     */
    DirectLight(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular, const glm::vec3 &direction)
        : Light(ambient, diffuse, specular)
    {
        glm::vec3 up(0.0f, 1.0f, 0.0f);

        /* Set the rotation according to the direction */
        setOrientation(glm::orientation(glm::normalize(direction), up));
    }

    /**
     * Destructor
     */
    ~DirectLight() {}
    /**
     * Specializes the type of projection matrix that this light needs
     * for shadow mapping
     *
     * Direct lights use orthogonal projections for the shadow mapping
     *
     * @return The orthogonal projection matrix for the direct light
     */
    const glm::mat4 &getProjectionMatrix() { return getOrthogonalMatrix(); }
};
