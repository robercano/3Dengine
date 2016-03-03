/**
 * @class	Object3D
 * @brief	Basic object 3D representation
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include <stdint.h>
#include <glm/glm.hpp>
#include <vector>
#include "Material.hpp"
#include "Texture.hpp"

class Object3D
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
		virtual ~Object3D() {}

        const std::vector< Object3D::VertexData > &getVertexData() const
        {
            return _objectData;
        }
        const std::vector< uint32_t > &getIndexData() const
        {
            return _objectIndices;
        }
        const std::vector< Material > &getMaterials() const
        {
            return _materials;
        }
        const std::vector< Texture > &getTextures() const
        {
            return _textures;
        }
        const std::vector< uint32_t > &getIndicesOffsets() const
        {
            return _indicesOffsets;
        }
        const std::vector< uint32_t > &getIndicesCount() const
        {
            return _indicesCount;
        }

    protected:
        Object3D() {}

        /**
         * Object data
         */
        std::vector<Object3D::VertexData> _objectData;
        std::vector<uint32_t>             _objectIndices;
        std::vector<Material>             _materials;
        std::vector<Texture>              _textures;
        std::vector<uint32_t>             _indicesOffsets;
        std::vector<uint32_t>             _indicesCount;
};
