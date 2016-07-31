#include "Sphere.hpp"
#include <glm/glm.hpp>
#include "Asset3DTransform.hpp"
#include "Circle.hpp"
#include "ProceduralUtils.hpp"

using namespace Procedural;

#define PI 3.14159265358979323846

Sphere::Sphere(float radius, const glm::vec3 &color, uint32_t numVertsLongitude, uint32_t numVertsLatitude)
    : _radius(radius), _color(color), _numVertsLongitude(numVertsLongitude), _numVertsLatitude(numVertsLatitude)
{
    AppendBentPlane(*this, (float)(2.0f * PI * _radius), (float)(2.0f * _radius), (float)(2.0f * PI), 0.0f, (float)(2.0f * PI),
                    _numVertsLongitude + 1, _numVertsLatitude);
    Asset3DTransform::SetUniqueMaterialFromColor(*this, _color);
}
