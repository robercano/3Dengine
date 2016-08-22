/**
 * @module  Sphere
 * @brief	Sphere procedural generation
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include <stdint.h>
#include "Model3D.hpp"

namespace Procedural
{
/**
 * Sphere class to generate a sphere
 */
class Sphere : public Model3D
{
  public:
    /**
     * Constructor
     *
     * @param numSides  Number of sides of the cylinder. Minimum is 4 sides, which
     *                  would yield a cube
     * @param color     Color for the default material
     */
    Sphere(float radius = 1.0f, const glm::vec3 &color = glm::vec3(1.0f, 1.0f, 1.0f), uint32_t numVertsLongitude = 20,
           uint32_t numVertsLatitude = 20);

    /**
     * Accessors
     */
    float getRadius() const { return _radius; }
    const glm::vec3 &getColor() const { return _color; }
    uint32_t getNumVertsLongitude() const { return _numVertsLongitude; }
    uint32_t getNumVertsLatitude() const { return _numVertsLatitude; }
  private:
    float _radius;
    glm::vec3 _color;
    uint32_t _numVertsLongitude;
    uint32_t _numVertsLatitude;
};
};
