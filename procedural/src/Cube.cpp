#include "Cube.hpp"
#include <glm/glm.hpp>
#include "Asset3DTransform.hpp"
#include "Logging.hpp"
#include "Plane.hpp"

using namespace Procedural;
using namespace Logging;

#define PI 3.14159265358979323846

Cube::Cube(float width, float height, float depth, const glm::vec3 &color, uint32_t numVertsWidth, uint32_t numVertsHeight,
           uint32_t numVertsDepth)
    : _width(width)
    , _height(height)
    , _depth(depth)
    , _color(color)
    , _numVertsWidth(numVertsWidth)
    , _numVertsHeight(numVertsHeight)
    , _numVertsDepth(numVertsDepth)
{
    float halfWidth = _width / 2.0f;
    float halfHeight = _height / 2.0f;
    float halfDepth = _depth / 2.0f;

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
        glm::vec2(_width, _depth),  /* Top */
        glm::vec2(_width, _depth),  /* Bottom */
        glm::vec2(_width, _height), /* Front */
        glm::vec2(_width, _height), /* Back */
        glm::vec2(_height, _depth), /* Left */
        glm::vec2(_height, _depth)  /* Right */
    };

    struct {
        uint32_t _width;
        uint32_t _height;
    } planeVerts[] = {
        {_numVertsWidth, _numVertsDepth},  /* Top */
        {_numVertsWidth, _numVertsDepth},  /* Bottom */
        {_numVertsWidth, _numVertsHeight}, /* Front */
        {_numVertsWidth, _numVertsHeight}, /* Back */
        {_numVertsHeight, _numVertsDepth}, /* Left */
        {_numVertsHeight, _numVertsDepth}  /* Right */
    };

    for (int i = 0; i < sizeof offsets / sizeof *offsets; ++i) {
        Plane plane(planeSizes[i].x, planeSizes[i].y, _color, planeVerts[i]._width, planeVerts[i]._height);

        /* Transform the original plane */
        Asset3DTransform::Rotate(plane, rotations[i]);
        Asset3DTransform::Translate(plane, offsets[i]);
        Asset3DTransform::AppendGeometryOnly(*this, plane);
    }

    Asset3DTransform::SetUniqueMaterialFromColor(*this, _color);
}
