/**
 * @module  Plane
 * @brief	Plane procedural generation
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include <stdint.h>
#include <glm/glm.hpp>
#include <vector>
#include "Model3D.hpp"

namespace procedural
{
/**
 * Plane class to generate axis aligned planes
 */
class Plane : public Model3D
{
  public:
    Plane(uint32_t horizontal = 2, uint32_t vertical = 2);
};
};
