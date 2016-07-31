/**
 * @module  Triangle
 * @brief	Triangle procedural generation
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#include "Triangle.hpp"
#include "Asset3DTransform.hpp"

using namespace Procedural;

Triangle::Triangle(const glm::vec3 &vertex0, const glm::vec3 &vertex1, const glm::vec3 &vertex2, const glm::vec3 &color) : _color(color)
{
    glm::vec3 normal = glm::normalize(glm::cross(vertex2 - vertex1, vertex0 - vertex1));

    _asset->_vertexData.resize(3);
    _asset->_vertexData[0].vertex = vertex0;
    _asset->_vertexData[0].normal = normal;
    _asset->_vertexData[1].vertex = vertex1;
    _asset->_vertexData[1].normal = normal;
    _asset->_vertexData[2].vertex = vertex2;
    _asset->_vertexData[2].normal = normal;

    _asset->_vertexIndices.resize(3);

    _asset->_vertexIndices[0] = 0;
    _asset->_vertexIndices[1] = 1;
    _asset->_vertexIndices[2] = 2;

    Asset3DTransform::SetUniqueMaterialFromColor(*this, _color);
}
