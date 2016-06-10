/**
 * @class	Model3D
 * @brief	Basic model 3D representation
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include <stdint.h>
#include <glm/glm.hpp>
#include <vector>
#include "Material.hpp"
#include "Object3D.hpp"
#include "Texture.hpp"

class Model3D : public Object3D
{
  public:
    typedef struct {
        glm::vec3 vertex;
        glm::vec3 normal;
        glm::vec2 uvcoord;
    } VertexData;

    /**
     * Destructor
     */
    virtual ~Model3D() {}
    void normalize();

    const std::vector<Model3D::VertexData> &getVertexData() const { return _modelData; }
    const std::vector<uint32_t> &getIndexData() const { return _modelIndices; }
    const std::vector<Material> &getMaterials() const { return _materials; }
    const std::vector<Texture> &getTextures() const { return _textures; }
    const std::vector<uint32_t> &getIndicesOffsets() const { return _indicesOffsets; }
    const std::vector<uint32_t> &getIndicesCount() const { return _indicesCount; }
  protected:
    Model3D() {}
    void _calculateCenterOfMass(glm::vec3 &cm);

    /**
     * Object data
     */
    std::vector<Model3D::VertexData> _modelData;
    std::vector<uint32_t> _modelIndices;
    std::vector<Material> _materials;
    std::vector<Texture> _textures;
    std::vector<uint32_t> _indicesOffsets;
    std::vector<uint32_t> _indicesCount;
};
