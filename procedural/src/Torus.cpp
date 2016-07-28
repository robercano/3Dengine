#include "Torus.hpp"
#include <glm/glm.hpp>
#include "Logging.hpp"
#include "ModelTransform.hpp"
#include "ProceduralUtils.hpp"

using namespace Procedural;
using namespace Logging;

#define PI 3.14159265358979323846

Torus::Torus(float outerRadius, float innerRadius, const glm::vec3 &color, uint32_t numVertsToroidal, uint32_t numVertsPoloidal)
    : _outerRadius(outerRadius)
    , _innerRadius(innerRadius)
    , _color(color)
    , _numVertsToroidal(numVertsToroidal)
    , _numVertsPoloidal(numVertsPoloidal)
{
    if (_numVertsToroidal < 3) {
        _numVertsToroidal = 3;
    }
    if (_numVertsPoloidal < 3) {
        _numVertsPoloidal = 3;
    }

    AppendBentPlane(*this, (float)((_outerRadius - _innerRadius) * 2.0f * PI), (float)(_outerRadius * 2.0f * PI), (float)(2.0f * PI),
                    (float)(2.0f * PI), 0.0f, _numVertsToroidal + 1, _numVertsPoloidal + 1);
    ModelTransform::SetUniqueMaterialFromColor(*this, _color);
}
