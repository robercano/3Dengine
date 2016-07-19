/**
 * @module  BentPlane
 * @brief	Bent plane procedural generation
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#include "BentPlane.hpp"
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include "Logging.hpp"
#include "ModelTransform.hpp"
#include "ProceduralUtils.hpp"

using namespace Logging;
using namespace Procedural;

#define PI 3.14159265358979323846

BentPlane::BentPlane(float width, float height, const glm::vec3 &color, float angle, uint32_t numVertsWidth, uint32_t numVertsHeight)
    : _width(width), _height(height), _color(color), _angle(angle), _numVertsWidth(numVertsWidth), _numVertsHeight(numVertsHeight)
{
    AppendBentPlane(*this, _width, _height, _angle, 0.0f, 0.0f, _numVertsWidth, _numVertsHeight);
    ModelTransform::SetUniqueMaterialFromColor(*this, _color);
}
