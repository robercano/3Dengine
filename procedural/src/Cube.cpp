#include "Cube.hpp"
#include <glm/glm.hpp>
#include "Logging.hpp"
#include "ModelTransform.hpp"
#include "Plane.hpp"

using namespace Procedural;
using namespace Logging;

#define PI 3.14159265358979323846

Cube::Cube(uint32_t numVertices, const glm::vec3 &color)
{
    glm::vec3 offsets[] = {
        glm::vec3(0.0f, 0.5f, 0.0f),  /* Top */
        glm::vec3(0.0f, -0.5f, 0.0f), /* Bottom */
        glm::vec3(0.0f, 0.0f, 0.5f),  /* Front */
        glm::vec3(0.0f, 0.0f, -0.5f), /* Back */
        glm::vec3(-0.5f, 0.0f, 0.0f), /* Left */
        glm::vec3(0.5f, 0.0f, 0.0f),  /* Right */
    };

    glm::vec3 rotations[] = {
        glm::vec3(0.0, 0.0, 0.0),       /* Top */
        glm::vec3(PI, 0.0, 0.0),        /* Bottom */
        glm::vec3(PI / 2.0, 0.0, 0.0),  /* Front */
        glm::vec3(-PI / 2.0, 0.0, 0.0), /* Back */
        glm::vec3(0.0, 0.0, PI / 2.0),  /* Left */
        glm::vec3(0.0, 0.0, -PI / 2.0), /* Right */
    };

    for (int i = 0; i < sizeof offsets / sizeof *offsets; ++i) {
        Plane plane(numVertices, color);

        /* Transform the original plane */
        ModelTransform::Rotate(plane, rotations[i]);
        ModelTransform::Translate(plane, offsets[i]);
        ModelTransform::Append(*this, plane);
    }
}
