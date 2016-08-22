/**
 * @module  ProceduralUtils
 * @brief	Functions to aid the procedural generation of models
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include <stdint.h>
#include <glm/glm.hpp>

class Asset3D;

namespace Procedural
{
/**
 * Function to generate planes with a bending angle on both
 * x and z-axis
 *
 * The angleWidth angle bends the plane around the z-axis, and can be used
 * to generate cylinder like planes.
 *
 * The angleHeight angle bends the plane around the x-axis, but in this case
 * by rotating the yz-plane around the x-axis and calculating the points. This
 * allows to bent a cylinder-like plane into a torus
 *
 * The angleRadius angle modifies the distance of the vertex to the center
 * along the z-axis. Together with angleWidth can be used to create spheres
 * when used with 2*PI.
 *
 * @param model       Model to append the bent plane to
 * @param width       Width of the plane along the x-axis. If 'angleWidth' is not
 *                    0.0 the width will be used to calculate the radius of the
 *                    curve along the x-axis. This means that width becomes the
 *                    length of the circumference on the xy-plane
 * @param height      Height of the plane along the z-axis. If 'angleHeight' is not
 *                    0.0 the height will be used to calculate the radius of the curve
 *                    along the z-axis. This means that height becomes the length
 *                    of the circumference on the zy-plane
 * @param angleWidth  Radians of the arc of circumference with length 'width', used to
 *                    calculate the curvature of the plane along the x-axis. If 2*PI
 *                    is used then a cylinder curved around the x-axis is generated
 * @param angleHeight Radians of the arc of circumference with length 'height', used
 *                    to calculate the amount of rotation around the x-axis to achieve
 *                    curvature on the z-axis. When used together with 'angleWidth'
 *                    toroidal figures can be generated. If its value is 2*PI and
 *                    the value of 'angleWidth' is 2*PI then a torus is generated
 * @param angleRadius Affects the maximum radius from the vertices to the center of
 *                    the figure. If 0.0 all vertices have same distance to the xz-plane.
 *                    If higher than 0.0 then the farther a vertex is from the center,
 *                    the closer it will be to the xz-plane. With a value of 2*PI along
 *                    a value of 2*PI for 'angleWidth' a sphere will be generated
 */
void AppendBentPlane(Asset3D &asset, float width, float height, float angleWidth, float angleHeight, float angleRadius,
                     uint32_t numVertsWidth, uint32_t numVertsHeight);
};
