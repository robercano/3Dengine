/**
 * @module  Plane
 * @brief	Procedural plane generation
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#include "Plane.hpp"
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include "Asset3DTransform.hpp"
#include "Logging.hpp"
#include "ProceduralUtils.hpp"

using namespace Logging;
using namespace Procedural;

#define PI 3.14159265358979323846

Plane::Plane(float width, float height, const glm::vec3 &color, uint32_t numVertsWidth, uint32_t numVertsHeight)
    : _width(width), _height(height), _color(color), _numVertsWidth(numVertsWidth), _numVertsHeight(numVertsHeight)
{
    AppendBentPlane(*this, _width, _height, 0.0f, 0.0f, 0.0f, _numVertsWidth, _numVertsHeight);

    Asset3DTransform::SetUniqueMaterialFromColor(*this, _color);
}
