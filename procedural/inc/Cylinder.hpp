/**
 * @module  Cylinder
 * @brief	Cylinder procedural generation
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include <stdint.h>
#include "Model3D.hpp"

namespace Procedural
{
/**
 * Cylinder class to generate a cube with axis aligned planes
 */
class Cylinder : public Model3D
{
  public:
    /**
     * Constructor
     *
     * @param numSides  Number of sides of the cylinder. Minimum is 4 sides, which
     *                  would yield a cube
     * @param color     Color for the default material
     */
    Cylinder(float radius = 1.0f, float height = 1.0f,
             const glm::vec3 &color = glm::vec3(1.0f, 1.0f, 1.0f),
             uint32_t numVertsCap = 20, uint32_t numVertsHeight = 2);

  private:
    void _buildBody();

    float _radius;
    float _height;
    glm::vec3 _color;
    uint32_t _numVertsCap;
    uint32_t _numVertsHeight;
};
};
