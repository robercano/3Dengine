#include "Cylinder.hpp"
#include <glm/glm.hpp>
#include "Asset3DTransform.hpp"
#include "Circle.hpp"
//#include "ProceduralUtils.hpp"

using namespace Procedural;

#define PI 3.14159265358979323846

Cylinder::Cylinder(float radius, float height, const glm::vec3 &color, uint32_t numVertsCap, uint32_t numVertsHeight)
    : _radius(radius), _height(height), _color(color), _numVertsCap(numVertsCap), _numVertsHeight(numVertsHeight)
{
    Circle bottomCap(_radius, _color, _numVertsCap);
    Circle topCap(_radius, _color, _numVertsCap);

    /* Bottom cap */
    Asset3DTransform::Rotate(bottomCap, glm::vec3(PI, 0.0f, 0.0f));
    Asset3DTransform::AppendGeometryOnly(*this, bottomCap);

    /* Upper cap */
    Asset3DTransform::Translate(topCap, glm::vec3(0.0f, height, 0.0f));
    Asset3DTransform::AppendGeometryOnly(*this, topCap);

    /* Body */
    Asset3D body;
    AppendBentPlane(body, (float)(2.0f * PI * _radius), _height, (float)(2.0 * PI), 0.0f, 0.0f, _numVertsCap + 1, _numVertsHeight);

    Asset3DTransform::Translate(body, glm::vec3(0.0f, -_radius, 0.0f));
    Asset3DTransform::Rotate(body, glm::vec3(-PI / 2.0f, 0.0f, 0.0f));
    Asset3DTransform::Translate(body, glm::vec3(0.0f, _radius, 0.0f));
    Asset3DTransform::Rotate(body, glm::vec3(0.0f, PI / _numVertsCap, 0.0f));
    Asset3DTransform::AppendGeometryOnly(*this, body);

    /* Generate the material */
    Asset3DTransform::SetUniqueMaterialFromColor(*this, _color);
}
