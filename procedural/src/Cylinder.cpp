#include "Cylinder.hpp"
#include <glm/glm.hpp>
#include "Circle.hpp"
#include "ModelTransform.hpp"
#include "Plane.hpp"

using namespace Procedural;

#define PI 3.14159265358979323846

void Cylinder::_buildBody()
{
    uint32_t bodyVertexBase = _modelData.size();
    uint32_t count;

    /* The body will have 'numVertsCap' * 'numVertsHeight' vertices */
    _modelData.resize(bodyVertexBase + _numVertsCap * _numVertsHeight);

    Model3D::VertexData *data = &_modelData[bodyVertexBase];

    /* Body */
    count = 0;

    float heightStep = _height / (_numVertsHeight - 1);
    for (unsigned int j = 0; j < _numVertsHeight; ++j) {
        for (unsigned int i = 0; i < _numVertsCap; ++i) {
            float angle = (float)(2 * PI * i / _numVertsCap);
            float ringHeight = j * heightStep;

            data[count].vertex = glm::vec3(_radius * glm::cos(angle), ringHeight, _radius * glm::sin(angle));
            data[count].normal = glm::normalize(data[count].vertex);
            count++;
        }
    }

    uint32_t bodyIndicesBase = _modelIndices.size();

    /* Generate the indices now */
    /* There are 'numVertsCap' * ('numVertsHeight' - 1) * 2 triangles for the body
     * with 3 indices per triangle */
    _modelIndices.resize(bodyIndicesBase + (_numVertsCap * (_numVertsHeight - 1) * 2) * 3);

    uint32_t *index = &_modelIndices[bodyIndicesBase];
    count = 0;

    /* Body */
    for (unsigned int j = 0; j < _numVertsHeight - 1; ++j) {
        unsigned int i = 0;
        for (; i < _numVertsCap - 1; ++i) {
            index[count++] = j * _numVertsCap + bodyVertexBase + i;
            index[count++] = j * _numVertsCap + bodyVertexBase + _numVertsCap + i;
            index[count++] = j * _numVertsCap + bodyVertexBase + i + 1;

            index[count++] = j * _numVertsCap + bodyVertexBase + i + 1;
            index[count++] = j * _numVertsCap + bodyVertexBase + _numVertsCap + i;
            index[count++] = j * _numVertsCap + bodyVertexBase + _numVertsCap + i + 1;
        }
        /* Last quad for this ring: 'i' contains the last position */
        index[count++] = j * _numVertsCap + bodyVertexBase + i;
        index[count++] = j * _numVertsCap + bodyVertexBase + _numVertsCap + i;
        index[count++] = j * _numVertsCap + bodyVertexBase + 0;

        index[count++] = j * _numVertsCap + bodyVertexBase + 0;
        index[count++] = j * _numVertsCap + bodyVertexBase + _numVertsCap + i;
        index[count++] = j * _numVertsCap + bodyVertexBase + _numVertsCap + 0;
    }
}

Cylinder::Cylinder(float radius, float height, const glm::vec3 &color, uint32_t numVertsCap, uint32_t numVertsHeight)
    : _radius(radius), _height(height), _color(color), _numVertsCap(numVertsCap), _numVertsHeight(numVertsHeight)
{
    Circle bottomCap(_radius, _color, _numVertsCap);
    Circle topCap(_radius, _color, _numVertsCap);
    Plane body(2.0f * PI * _radius, height, color, 2.0 * PI, 0.0f, _numVertsCap + 1, _numVertsHeight);

    /* Bottom cap */
    ModelTransform::Rotate(bottomCap, glm::vec3(PI, 0.0f, 0.0f));
    ModelTransform::Append(*this, bottomCap);

    /* Upper cap */
    ModelTransform::Translate(topCap, glm::vec3(0.0f, height, 0.0f));
    ModelTransform::Append(*this, topCap);

    /* Body */
    ModelTransform::Translate(body, glm::vec3(0.0f, -_radius, 0.0f));
    ModelTransform::Rotate(body, glm::vec3(-PI / 2.0f, 0.0f, 0.0f));
    ModelTransform::Translate(body, glm::vec3(0.0f, _radius, 0.0f));
    ModelTransform::Rotate(body, glm::vec3(0.0f, PI / _numVertsCap, 0.0f));
    ModelTransform::Append(*this, body);

    /* Only one set of indices */
    _indicesOffsets.push_back(0);
    _indicesCount.push_back(_modelIndices.size());
}
