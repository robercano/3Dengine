/**
 * @module  Triangle
 * @brief	Triangle procedural generation
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include <stdint.h>
#include "Model3D.hpp"

namespace Procedural
{
/**
 * Triangle class to generate a cube with axis aligned planes
 */
class Triangle : public Model3D
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
    Triangle(const glm::vec3 &vertex0,
             const glm::vec3 &vertex1,
             const glm::vec3 &vertex2,
             const glm::vec3 &color = glm::vec3(1.0f, 1.0f, 1.0f));

    /**
     * Accesors
     */
    const glm::vec3 &getColor() const { return _color; }

  private:
    glm::vec3 _color;
};
};
