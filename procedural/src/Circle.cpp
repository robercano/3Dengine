#include "Circle.hpp"

using namespace Procedural;

#define PI 3.14159265358979323846

Circle::Circle(float radius, const glm::vec3 &color, uint32_t numVertsSide) : _radius(radius), _color(color), _numVertsSide(numVertsSide)
{
    glm::vec3 normal(0.0f, 1.0f, 0.0f);
    uint32_t count = 0;

    /* A circle has a center vertex and '_numVertsSide' border vertices */
    _asset->_vertexData.resize(1 + _numVertsSide);
    Asset3D::VertexData *data = &_asset->_vertexData[0];

    /* Vertices */
    data[count].vertex = glm::vec3(0.0f, 0.0f, 0.0f);
    data[count].normal = normal;
    count++;

    for (unsigned int i = 0; i < _numVertsSide; ++i) {
        float angle = (float)(2.0f * PI * i / _numVertsSide);

        data[count].vertex = glm::vec3(_radius * glm::cos(angle), 0.0f, _radius * glm::sin(angle));
        data[count].normal = normal;
        count++;
    }

    /* Faces */

    /* A cap has '_numVertsSide' triangles */
    _asset->_vertexIndices.resize(_numVertsSide * 3);
    uint32_t *index = &_asset->_vertexIndices[0];

    /* Bottom lid */
    count = 0;
    for (unsigned int i = 0; i < _numVertsSide - 1; ++i) {
        index[count++] = 0;
        index[count++] = 2 + i;
        index[count++] = 1 + i;
    }

    /* Bottom lid last triangle */
    index[count++] = 0;
    index[count++] = 1;
    index[count++] = _numVertsSide;

    /* Add a default material */
    _asset->_materials.push_back(Material());

    /* Add a default texture */
    uint8_t rgb[3] = {(uint8_t)(color.r * 255), (uint8_t)(color.g * 255), (uint8_t)(color.b * 255)};
    _asset->_textures.push_back(Texture(rgb, 1, 1, 8));

    /* Only one set of indices */
    _asset->_indicesOffsets.push_back(0);
    _asset->_indicesCount.push_back(_asset->_vertexIndices.size());
}
