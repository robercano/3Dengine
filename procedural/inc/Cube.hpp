/**
 * @module  Cube
 * @brief	Cube procedural generation
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include <stdint.h>
#include "Model3D.hpp"

namespace Procedural
{
/**
 * Cube class to generate axis aligned planes
 */
class Cube : public Model3D
{
  public:
    Cube(uint32_t horizontal = 2, uint32_t vertical = 2);
};
};
