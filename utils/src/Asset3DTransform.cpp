/**
 * @class Asset3DTransform
 * @brief Transforms the vertex data of a Asset3D and stores the result
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#include "Asset3DTransform.hpp"
#include <glm/gtx/quaternion.hpp>
#include <map>
#include "Logging.hpp"

using namespace Logging;

void Asset3DTransform::Rotate(Asset3D &asset, const glm::vec3 eulerAngles)
{
    glm::mat4 rotation = glm::toMat4(glm::quat(eulerAngles));

    for (std::vector<Asset3D::VertexData>::iterator it = asset._vertexData.begin(); it != asset._vertexData.end(); ++it) {
        it->vertex = glm::vec3(rotation * glm::vec4(it->vertex, 1.0f));
        it->normal = glm::mat3(rotation) * it->normal;
    }
}

void Asset3DTransform::Translate(Asset3D &asset, const glm::vec3 offsets)
{
    for (std::vector<Asset3D::VertexData>::iterator it = asset._vertexData.begin(); it != asset._vertexData.end(); ++it) {
        it->vertex += offsets;
    }
}

void Asset3DTransform::Append(Asset3D &to, const Asset3D &from)
{
    uint32_t origIndexSize = to._vertexIndices.size();

    AppendGeometryOnly(to, from);

    /* The vertex data, materials, textures and indices count can be appended directly,
     * as they are independant of the _vertexData size */
    to._materials.insert(to._materials.end(), from.getMaterials().begin(), from.getMaterials().end());
    to._textures.insert(to._textures.end(), from.getTextures().begin(), from.getTextures().end());
    to._indicesCount.insert(to._indicesCount.end(), from.getIndicesCount().begin(), from.getIndicesCount().end());

    /* Copy the vertices ofssets. We need to add to the offsets the original size of the
     * _vertexData array in the 'to' asset */
    for (std::vector<uint32_t>::const_iterator it = from._indicesOffsets.begin(); it != from._indicesOffsets.end(); ++it) {
        to._indicesOffsets.push_back(*it + origIndexSize);
    }
}

void Asset3DTransform::AppendGeometryOnly(Asset3D &to, const Asset3D &from)
{
    uint32_t origDataSize = to._vertexData.size();

    /* The vertex data, materials, textures and indices count can be appended directly,
     * as they are independant of the _vertexData size */
    to._vertexData.insert(to._vertexData.end(), from.getVertexData().begin(), from.getVertexData().end());

    /* Copy the vertices indices. We need to add to the indices the original size of the
     * _vertexData array in the 'to' asset */
    for (std::vector<uint32_t>::const_iterator it = from._vertexIndices.begin(); it != from._vertexIndices.end(); ++it) {
        to._vertexIndices.push_back(*it + origDataSize);
    }
}

void Asset3DTransform::SetUniqueMaterial(Asset3D &asset, const Material &material, const Texture &texture)
{
    /* Clear all previous data */
    asset._materials.clear();
    asset._textures.clear();
    asset._indicesOffsets.clear();
    asset._indicesCount.clear();

    asset._materials.push_back(material);
    asset._textures.push_back(texture);

    /* Only one set of indices */
    asset._indicesOffsets.push_back(0);
    asset._indicesCount.push_back(asset._vertexIndices.size());
}

void Asset3DTransform::SetUniqueMaterialFromColor(Asset3D &asset, const glm::vec3 &color)
{
    uint8_t rgb[3] = {(uint8_t)(color.r * 255), (uint8_t)(color.g * 255), (uint8_t)(color.b * 255)};

    SetUniqueMaterial(asset, Material(), Texture(rgb, 1, 1, 8));
}

void Asset3DTransform::RecalculateNormals(Asset3D &asset)
{
    /* Loop the asset indices and create a map for each vertex
     * containing the normals of the faces it touches */
    std::map<uint32_t, std::vector<glm::vec3> > normalsMap;

    uint32_t *index = &asset._vertexIndices[0];

    for (uint32_t i = 0; i < asset._vertexIndices.size(); i += 3) {
        /* Calculate the normal for the face */
        glm::vec3 a = asset._vertexData[index[i]].vertex - asset._vertexData[index[i + 1]].vertex;
        glm::vec3 b = asset._vertexData[index[i + 2]].vertex - asset._vertexData[index[i + 1]].vertex;
        glm::vec3 normal = glm::cross(b, a);

        normalsMap[index[i]].push_back(normal);
        normalsMap[index[i + 1]].push_back(normal);
        normalsMap[index[i + 2]].push_back(normal);
    }

    /* Now loop the normals map and calculate the average */
    for (std::map<uint32_t, std::vector<glm::vec3> >::iterator it = normalsMap.begin(); it != normalsMap.end(); ++it) {
        glm::vec3 normal(0.0f);
        for (std::vector<glm::vec3>::iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2) {
            normal += *it2;
        }

        /* Divide by the number of normals in the average and set it to
         * the right vertex */
        asset._vertexData[it->first].normal = glm::normalize(normal);
    }
}
