/**
 * @module  Circle
 * @brief	Circle procedural generation
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include <stdint.h>
#include <glm/glm.hpp>
#include "Model3D.hpp"

namespace Procedural
{
/**
 * Circle class to generate a flat circle
 */
class Circle : public Model3D
{
  public:
    /**
     * Constructor
     *
     */
    Circle(float radius = 1.0f, const glm::vec3 &color = glm::vec3(1.0f, 1.0f, 1.0f), uint32_t numVertesSide = 20);

  private:
    float _radius;
    glm::vec3 _color;
    uint32_t _numVertsSide;
};
};
