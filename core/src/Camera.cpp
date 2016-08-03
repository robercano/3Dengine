/**
 * @class	Camera
 * @brief	Camera object. It can be used to project objects onto it through the
 *          Projection interface, allowing for perspective and orthogonal projections. It
 *          inherits from Object3D to allow movement of the camera through interpolators
 *          or Movement objects
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#include "Camera.hpp"
#include <stdint.h>
#include <vector>
#include "Logging.hpp"

using namespace Logging;

void Camera::recalculateFrustum(void)
{
    glm::mat4 MVP = getPerspectiveMatrix() * getViewMatrix();

#if 0 /* Left here to understand how the planes are calculated \
         in the loop below */
    float planesTerms[MAX_PLANES][4] = {
        {MVP[0][0] + MVP[0][3], MVP[1][0] + MVP[1][3], MVP[2][0] + MVP[2][3], MVP[3][0] + MVP[3][3]},     /* Left plane */
        {-MVP[0][0] + MVP[0][3], -MVP[1][0] + MVP[1][3], -MVP[2][0] + MVP[2][3], -MVP[3][0] + MVP[3][3]}, /* Right plane */
        {MVP[0][1] + MVP[0][3], MVP[1][1] + MVP[1][3], MVP[2][1] + MVP[2][3], MVP[3][1] + MVP[3][3]},     /* Bottom plane */
        {-MVP[0][1] + MVP[0][3], -MVP[1][1] + MVP[1][3], -MVP[2][1] + MVP[2][3], -MVP[3][1] + MVP[3][3]}, /* Top plane */
        {MVP[0][2] + MVP[0][3], MVP[1][2] + MVP[1][3], MVP[2][2] + MVP[2][3], MVP[3][2] + MVP[3][3]},     /* Near plane */
        {-MVP[0][2] + MVP[0][3], -MVP[1][2] + MVP[1][3], -MVP[2][2] + MVP[2][3], -MVP[3][2] + MVP[3][3]}, /* Far plane */
    };
#endif

    for (int i = 0; i < MAX_PLANES; ++i) {
        float sign = (i % 2) ? -1.0f : 1.0f;

        glm::vec3 normal = glm::vec3(sign * MVP[0][i / 2] + MVP[0][3], sign * MVP[1][i / 2] + MVP[1][3], sign * MVP[2][i / 2] + MVP[2][3]);

        _frustumPlanes[i] = glm::vec4(normal, sign * MVP[3][i / 2] + MVP[3][3]) / glm::length(normal);
    }
}

bool Camera::isObjectVisible(Object3D &object)
{
    float radius = object.getBoundingSphere().getRadius();

    for (int i = 0; i < MAX_PLANES; ++i) {
        /* Check the sphere */
        if (glm::dot(_frustumPlanes[i], glm::vec4(object.getPosition(), 1.0f)) < -radius) {
            return false;
        }
    }
    return true;
}
