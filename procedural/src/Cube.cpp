#include "Cube.hpp"
#include <glm/glm.hpp>
#include "Logging.hpp"
#include "ModelTransform.hpp"
#include "Plane.hpp"

using namespace Procedural;
using namespace Logging;

#define PI 3.14159265358979323846

Cube::Cube(float width, float height, float depth, const glm::vec3 &color, uint32_t numVertsWidth, uint32_t numVertsHeight,
           uint32_t numVertsDepth)
{
    float halfWidth = width / 2.0f;
    float halfHeight = height / 2.0f;
    float halfDepth = depth / 2.0f;

    glm::vec3 offsets[] = {
        glm::vec3(0.0f, halfHeight, 0.0f),  /* Top */
        glm::vec3(0.0f, -halfHeight, 0.0f), /* Bottom */
        glm::vec3(0.0f, 0.0f, halfDepth),   /* Front */
        glm::vec3(0.0f, 0.0f, -halfDepth),  /* Back */
        glm::vec3(-halfWidth, 0.0f, 0.0f),  /* Left */
        glm::vec3(halfWidth, 0.0f, 0.0f),   /* Right */
    };

    glm::vec3 rotations[] = {
        glm::vec3(0.0, 0.0, 0.0),       /* Top */
        glm::vec3(PI, 0.0, 0.0),        /* Bottom */
        glm::vec3(PI / 2.0, 0.0, 0.0),  /* Front */
        glm::vec3(-PI / 2.0, 0.0, 0.0), /* Back */
        glm::vec3(0.0, 0.0, PI / 2.0),  /* Left */
        glm::vec3(0.0, 0.0, -PI / 2.0), /* Right */
    };

    glm::vec2 planeSizes[] = {
        glm::vec2(width, depth),  /* Top */
        glm::vec2(width, depth),  /* Bottom */
        glm::vec2(width, height), /* Front */
        glm::vec2(width, height), /* Back */
        glm::vec2(height, depth), /* Left */
        glm::vec2(height, depth)  /* Right */
    };

	struct {
		uint32_t width;
		uint32_t height;
	} planeVerts[] = {
		{ numVertsWidth, numVertsDepth  }, /* Top */
		{ numVertsWidth, numVertsDepth  }, /* Bottom */
		{ numVertsWidth, numVertsHeight }, /* Front */
		{ numVertsWidth, numVertsHeight }, /* Back */
		{ numVertsHeight, numVertsDepth }, /* Left */
		{ numVertsHeight, numVertsDepth }  /* Right */
    };

    for (int i = 0; i < sizeof offsets / sizeof *offsets; ++i) {
        Plane plane(planeSizes[i].x, planeSizes[i].y, color, 0.0f, planeVerts[i].width, planeVerts[i].height);

        /* Transform the original plane */
        ModelTransform::Rotate(plane, rotations[i]);
        ModelTransform::Translate(plane, offsets[i]);
        ModelTransform::Append(*this, plane);
    }
}
