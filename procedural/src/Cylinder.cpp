#include "Cylinder.hpp"
#include <glm/glm.hpp>
#include "Logging.hpp"
#include "ModelTransform.hpp"

using namespace Procedural;
using namespace Logging;

#define PI 3.14159265358979323846

Cylinder::Cylinder(float radius, float height, const glm::vec3 &color, uint32_t numVertsCap, uint32_t numVertsHeight)
{
    uint32_t count = 0;
    float halfHeight = height / 2.0f;

    glm::vec3 topCenter(0.0f, halfHeight, 0.0f);
    glm::vec3 bottomCenter(0.0f, -halfHeight, 0.0f);
    glm::vec3 topNormal = glm::normalize(topCenter);
    glm::vec3 bottomNormal = glm::normalize(bottomCenter);

    /* The cylinder will be conformed by the body and the 2 caps. Each cap will
     * have a center vertex and 'numVertsCap' vertices. Then the body will have
     * 'numVertsCap' * 'numVertsHeight' vertices */
    _modelData.resize(2 * (1 + numVertsCap) + numVertsCap * numVertsHeight);

    /* There are 'numVertsCap' triangles per cap
     * and 'numVertsCap' * ('numVertsHeight' - 1) * 2 triangles for the body
     * with 3 indices per triangle */
    _modelIndices.resize((numVertsCap +                             /* Top cap */
                          numVertsCap +                             /* Bottom cap */
                          numVertsCap * (numVertsHeight - 1) * 2) * /* Body */
                         3 /* Indices per triangle */);

    Model3D::VertexData *data = &_modelData[0];

    /* Upper cap */
    uint32_t upperCapBase = count;

    data[count].vertex = topCenter;
    data[count].normal = topCenter;
    count++;

    for (unsigned int i = 0; i < numVertsCap; ++i) {
        float angle = 2 * PI * i / numVertsCap;

        data[count].vertex = glm::vec3(radius * glm::cos(angle), 0.0f, radius * glm::sin(angle)) + topCenter;
        data[count].normal = topNormal;
        count++;
    }
    /* Bottom cap */
    uint32_t bottomCapBase = count;

    data[count].vertex = bottomCenter;
    data[count].normal = bottomCenter;
    count++;

    for (unsigned int i = 0; i < numVertsCap; ++i) {
        float angle = 2 * PI * i / numVertsCap;

        data[count].vertex = glm::vec3(radius * glm::cos(angle), 0.0f, radius * glm::sin(angle)) + bottomCenter;
        data[count].normal = bottomNormal;
        count++;
    }
    /* Body */
    uint32_t bodyBase = count;

    float heightStep = height / (numVertsHeight - 1);
    for (unsigned int j = 0; j < numVertsHeight; ++j) {
        for (unsigned int i = 0; i < numVertsCap; ++i) {
            float angle = 2 * PI * i / numVertsCap;
            float ringHeight = halfHeight - j * heightStep;

            data[count].vertex = glm::vec3(radius * glm::cos(angle), ringHeight, radius * glm::sin(angle));
            data[count].normal = glm::normalize(data[count].vertex);
            count++;
        }
    }

    /* Generate the indices now */
    uint32_t *index = &_modelIndices[0];
    count = 0;

    /* Upper lid */
    for (int i = 0; i < numVertsCap - 1; ++i) {
        index[count++] = upperCapBase;
        index[count++] = upperCapBase + 2 + i;
        index[count++] = upperCapBase + 1 + i;
    }
    /* Upper lid last triangle */
    index[count++] = upperCapBase;
    index[count++] = upperCapBase + 1;
    index[count++] = upperCapBase + numVertsCap;

    /* Bottom lid */
    for (int i = 0; i < numVertsCap - 1; ++i) {
        index[count++] = bottomCapBase;
        index[count++] = bottomCapBase + 1 + i;
        index[count++] = bottomCapBase + 2 + i;
    }
    /* Bottom lid last triangle */
    index[count++] = bottomCapBase;
    index[count++] = bottomCapBase + numVertsCap;
    index[count++] = bottomCapBase + 1;

    /* Sides */
    for (int j = 0; j < numVertsHeight - 1; ++j) {
        int i = 0;
        for (; i < numVertsCap - 1; ++i) {
            index[count++] = j * numVertsCap + bodyBase + i;
            index[count++] = j * numVertsCap + bodyBase + i + 1;
            index[count++] = j * numVertsCap + bodyBase + numVertsCap + i;

            index[count++] = j * numVertsCap + bodyBase + i + 1;
            index[count++] = j * numVertsCap + bodyBase + numVertsCap + i + 1;
            index[count++] = j * numVertsCap + bodyBase + numVertsCap + i;
        }
        /* Last quad for this ring: 'i' contains the last position */
        index[count++] = j * numVertsCap + bodyBase + i;
        index[count++] = j * numVertsCap + bodyBase + 0;
        index[count++] = j * numVertsCap + bodyBase + numVertsCap + i;

        index[count++] = j * numVertsCap + bodyBase + 0;
        index[count++] = j * numVertsCap + bodyBase + numVertsCap + 0;
        index[count++] = j * numVertsCap + bodyBase + numVertsCap + i;
    }

    _materials.push_back(Material());

    /* Add a default texture */
    uint8_t rgb[3] = {(uint8_t)(color.r * 255), (uint8_t)(color.g * 255), (uint8_t)(color.b * 255)};
    _textures.push_back(Texture(rgb, 1, 1, 8));

    /* Only one set of indices */
    _indicesOffsets.push_back(0);
    _indicesCount.push_back(_modelIndices.size());
}
