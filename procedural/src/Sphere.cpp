#include "Sphere.hpp"
#include <glm/glm.hpp>
#include "Circle.hpp"
#include "ModelTransform.hpp"
#include "ProceduralUtils.hpp"

using namespace Procedural;

#define PI 3.14159265358979323846

Sphere::Sphere(float radius, const glm::vec3 &color, uint32_t numVertsLongitude, uint32_t numVertsLatitude)
    : _radius(radius), _color(color), _numVertsLongitude(numVertsLongitude), _numVertsLatitude(numVertsLatitude)
{
    AppendBentPlane(*this, 2.0f * PI * radius, 2.0f * radius, color, 2.0f * PI, 0.0f, 2.0f * PI, numVertsLongitude + 1, numVertsLatitude);
}
