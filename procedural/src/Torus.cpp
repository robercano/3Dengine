#include "Torus.hpp"
#include <glm/glm.hpp>
#include "Logging.hpp"
#include "ModelTransform.hpp"
#include "Plane.hpp"

using namespace Procedural;
using namespace Logging;

#define PI 3.14159265358979323846

Torus::Torus(float outerRadius, float innerRadius, const glm::vec3 &color, uint32_t numVertsToroidal, uint32_t numVertsPoloidal)
{
    if (numVertsToroidal < 3) {
        numVertsToroidal = 3;
    }
    if (numVertsPoloidal < 3) {
        numVertsPoloidal = 3;
    }

    Plane torus((outerRadius - innerRadius) * 2.0f * PI, outerRadius * 2.0f * PI, color, 2.0 * PI, 2.0 * PI, numVertsToroidal + 1,
                numVertsPoloidal + 1);

    ModelTransform::Append(*this, torus);
}