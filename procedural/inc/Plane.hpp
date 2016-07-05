/**
 * @module  Plane
 * @brief	Plane procedural generation
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
 * Plane class to generate axis aligned planes
 */
class Plane : public Model3D
{
  public:
    Plane(uint32_t numVertices = 2, const glm::vec3 &color = glm::vec3(1.0f, 1.0f, 1.0f));
};
};
