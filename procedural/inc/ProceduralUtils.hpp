/**
 * @module  ProceduralUtils
 * @brief	Functions to aid the procedural generation of models
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include <stdint.h>
#include <glm/glm.hpp>

class Model3D;

namespace Procedural
{
    /**
     * Function to generate planes with a bending angle on both
     * x and z-axis. The second bending angle, on the z-axis, can
     * be used to simple offset the y coordinate, to rotate the coordinate
     * around the x-axis or TODO
     */
    void AppendBentPlane(Model3D &model,
                         float width, float height, const glm::vec3 &color,
                         float angleWidth, float angleHeight, float angleRadius,
                         uint32_t numVertsWidth, uint32_t numVertsHeight);
};
