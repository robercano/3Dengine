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
 * Cube class to generate a cube with axis aligned planes
 */
class Cube : public Model3D
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
    Cube(float width = 1.0f, float height = 1.0f, float depth = 1.0f,
         const glm::vec3 &color = glm::vec3(1.0f, 1.0f, 1.0f),
         uint32_t numVertsWidth = 2, uint32_t numVertsHeight = 2, uint32_t numVertsDepth = 2);
};
};
