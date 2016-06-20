/**
 * @class	BoundingSphere
 * @brief   Bounding sphere containing all model vertices
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include <glm/glm.hpp>

class BoundingSphere
{
  public:
    /**
     * Constructor
     */
    BoundingSphere(float radius = 1.0f) : _radius(radius) {}
    float getRadius() { return _radius; }
    void setRadius(float radius) { _radius = radius; }
  private:
    float _radius; /**< Radius of the sphere from the center of the model that
                        contains all the model's vertices */
};
