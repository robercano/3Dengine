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

bool Camera::isObjectVisible(Object3D &object)
{
    glm::mat4 MVP = getPerspectiveMatrix() * getViewMatrix();

    float radius = object.getBoundingSphere().getRadius();

    /* TODO: Don't calculate the planes all the time */
    float planesTerms[6][4] = {
        {MVP[0][0] + MVP[0][3], MVP[1][0] + MVP[1][3], MVP[2][0] + MVP[2][3], MVP[3][0] + MVP[3][3]},     /* Left plane */
        {-MVP[0][0] + MVP[0][3], -MVP[1][0] + MVP[1][3], -MVP[2][0] + MVP[2][3], -MVP[3][0] + MVP[3][3]}, /* Right plane */
        {MVP[0][1] + MVP[0][3], MVP[1][1] + MVP[1][3], MVP[2][1] + MVP[2][3], MVP[3][1] + MVP[3][3]},     /* Bottom plane */
        {-MVP[0][1] + MVP[0][3], -MVP[1][1] + MVP[1][3], -MVP[2][1] + MVP[2][3], -MVP[3][1] + MVP[3][3]}, /* top plane */
        {MVP[0][2] + MVP[0][3], MVP[1][2] + MVP[1][3], MVP[2][2] + MVP[2][3], MVP[3][2] + MVP[3][3]},     /* Near plane */
        {-MVP[0][2] + MVP[0][3], -MVP[1][2] + MVP[1][3], -MVP[2][2] + MVP[2][3], -MVP[3][2] + MVP[3][3]}, /* Far plane */
    };
    const char *names[6] = {"left", "right", "bottom", "top", "near", "far"};

    log("  Projection", getPerspectiveMatrix());
    log("  View", getViewMatrix());
    log("  MVP", MVP);
    log("  Position", object.getPosition());
    glm::vec4 clip = MVP * glm::vec4(object.getPosition(), 1.0f);

    clip = clip / clip.w;

    log("  Clip", clip);
#if 0
    for (int i = 0; i < sizeof planesTerms / sizeof *planesTerms; ++i) {
        log("Plane %s\n", names[i]);

        glm::vec3 normal = glm::vec3(planesTerms[i][0], planesTerms[i][1], planesTerms[i][2]);
        log("    Normal", normal);
        uint32_t length = glm::length(normal);

        glm::vec4 plane = glm::vec4(normal, planesTerms[i][3]) / length;

        log("    Plane", plane);
        /* Check the sphere */
        float distance = glm::dot(plane, glm::vec4(object.getPosition(), 1.0f));

        log("    Distance: %f\n", distance);
        if (distance < 0) {
            return true;
        }
    }
#endif
    return true;
}
