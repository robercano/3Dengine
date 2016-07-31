/**
 * @module  Cube
 * @brief	Cube procedural generation
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include <stdint.h>
#include "Asset3D.hpp"

namespace Procedural
{
/**
 * Cube class to generate a cube with axis aligned planes
 */
class Cube : public Asset3D
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

    /**
     * Accessors
     */
    float getWidth() const { return _width; }
    float getHeight() const { return _height; }
    float getDepth() const { return _depth; }
    const glm::vec3 &getColor() const { return _color; }
    uint32_t getNumVertsWidth() const { return _numVertsWidth; }
    uint32_t getNumVertsHeight() const { return _numVertsHeight; }
    uint32_t getNumVertsDepth() const { return _numVertsDepth; }

  private:
    float _width;
    float _height;
    float _depth;
    glm::vec3 _color;
    uint32_t _numVertsWidth;
    uint32_t _numVertsHeight;
    uint32_t _numVertsDepth;
};
};
