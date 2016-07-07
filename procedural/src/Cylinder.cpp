#include "Cylinder.hpp"
#include <glm/glm.hpp>
#include "Circle.hpp"
#include "ModelTransform.hpp"

using namespace Procedural;

#define PI 3.14159265358979323846

Cylinder::Cylinder(float radius, float height, const glm::vec3 &color, uint32_t numVertsCap, uint32_t numVertsHeight)
    : _radius(radius), _height(_height), _color(color), _numVertsCap(numVertsCap), _numVertsHeight(numVertsHeight)
{
    Circle bottomCap(_radius, color, _numVertsCap);
    Circle topCap(_radius, color, _numVertsCap);
    uint32_t count = 0;

    /* Bottom cap */
    ModelTransform::Rotate(bottomCap, glm::vec3(PI, 0.0f, 0.0f));
    ModelTransform::Append(*this, bottomCap);

    /* Upper cap */
    ModelTransform::Translate(topCap, glm::vec3(0.0f, height, 0.0f));
    ModelTransform::Append(*this, topCap);

    uint32_t bodyVertexBase = _modelData.size();

    /* The body will have 'numVertsCap' * 'numVertsHeight' vertices */
    _modelData.resize(bodyVertexBase + numVertsCap * numVertsHeight);

    Model3D::VertexData *data = &_modelData[bodyVertexBase];

    /* Body */
    count = 0;

    float heightStep = height / (numVertsHeight - 1);
    for (unsigned int j = 0; j < numVertsHeight; ++j) {
        for (unsigned int i = 0; i < numVertsCap; ++i) {
            float angle = 2 * PI * i / numVertsCap;
            float ringHeight = j * heightStep;

            data[count].vertex = glm::vec3(radius * glm::cos(angle), ringHeight, radius * glm::sin(angle));
            data[count].normal = glm::normalize(data[count].vertex);
            count++;
        }
    }

    uint32_t bodyIndicesBase = _modelIndices.size();

    /* Generate the indices now */
    /* There are 'numVertsCap' * ('numVertsHeight' - 1) * 2 triangles for the body
     * with 3 indices per triangle */
    _modelIndices.resize(bodyIndicesBase + (numVertsCap * (numVertsHeight - 1) * 2) * 3);

    uint32_t *index = &_modelIndices[bodyIndicesBase];
    count = 0;

    /* Body */
    for (int j = 0; j < numVertsHeight - 1; ++j) {
        int i = 0;
        for (; i < numVertsCap - 1; ++i) {
            index[count++] = j * numVertsCap + bodyVertexBase + i;
            index[count++] = j * numVertsCap + bodyVertexBase + numVertsCap + i;
            index[count++] = j * numVertsCap + bodyVertexBase + i + 1;

            index[count++] = j * numVertsCap + bodyVertexBase + i + 1;
            index[count++] = j * numVertsCap + bodyVertexBase + numVertsCap + i;
            index[count++] = j * numVertsCap + bodyVertexBase + numVertsCap + i + 1;
        }
        /* Last quad for this ring: 'i' contains the last position */
        index[count++] = j * numVertsCap + bodyVertexBase + i;
        index[count++] = j * numVertsCap + bodyVertexBase + numVertsCap + i;
        index[count++] = j * numVertsCap + bodyVertexBase + 0;

        index[count++] = j * numVertsCap + bodyVertexBase + 0;
        index[count++] = j * numVertsCap + bodyVertexBase + numVertsCap + i;
        index[count++] = j * numVertsCap + bodyVertexBase + numVertsCap + 0;
    }

    _materials.push_back(Material());

    /* Add a default texture */
    uint8_t rgb[3] = {(uint8_t)(color.r * 255), (uint8_t)(color.g * 255), (uint8_t)(color.b * 255)};
    _textures.push_back(Texture(rgb, 1, 1, 8));

    /* Only one set of indices */
    _indicesOffsets.push_back(0);
    _indicesCount.push_back(_modelIndices.size());
}
