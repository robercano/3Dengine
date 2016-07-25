/**
 * @module  Triangle
 * @brief	Triangle procedural generation
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#include "Triangle.hpp"
#include "ModelTransform.hpp"

using namespace Procedural;

Triangle::Triangle(const glm::vec3 &vertex0, const glm::vec3 &vertex1, const glm::vec3 &vertex2, const glm::vec3 &color) : _color(color)
{
    glm::vec3 normal = glm::normalize(glm::cross(vertex2 - vertex1, vertex0 - vertex1));

    _modelData.resize(3);
    _modelData[0].vertex = vertex0;
    _modelData[0].normal = normal;
    _modelData[1].vertex = vertex1;
    _modelData[1].normal = normal;
    _modelData[2].vertex = vertex2;
    _modelData[2].normal = normal;

    _modelIndices.resize(3);

    _modelIndices[0] = 0;
    _modelIndices[1] = 1;
    _modelIndices[2] = 2;

    ModelTransform::SetUniqueMaterialFromColor(*this, _color);
}
