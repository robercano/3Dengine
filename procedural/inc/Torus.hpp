/**
 * @module  Torus
 * @brief	Torus procedural generation
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include <stdint.h>
#include "Asset3D.hpp"

namespace Procedural
{
/**
 * Torus class to generate a procedural Torus
 */
class Torus : public Asset3D
{
  public:
    /**
     * Constructor
     *
     * @param numVertices  Number of vertices to use for the cube's faces
     *                     generation. Each face will have numVertices*numVertices
     *                     vertices
     * @param color        Color for the default material
     */
    Torus(float outerRadius = 10.0f, float innerRadius = 1.0f,
         const glm::vec3 &color = glm::vec3(1.0f, 1.0f, 1.0f),
         uint32_t numVertsToroidal = 20, uint32_t numVertsPoloidal = 20);

    /**
     * Accessors
     */
    float getOuterRadius() const { return _outerRadius; }
    float getInnerRadius() const { return _innerRadius; }
    const glm::vec3 &getColor() const { return _color; }
    uint32_t getNumVertsToroidal() const { return _numVertsToroidal; }
    uint32_t getNumVertsPoloidal() const { return _numVertsPoloidal; }

  private:
    float _outerRadius;
    float _innerRadius;
    glm::vec3 _color;
    uint32_t _numVertsToroidal;
    uint32_t _numVertsPoloidal;
};
};
