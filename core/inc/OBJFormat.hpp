/**
 * @class OBJLoader
 * @brief OBJ format loader
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>

#include "Object3D.hpp"

class OBJFormat : public Object3D
{
    public:
        /** Object3D methods */
        const Object3D::VertexData *getVertexData() const;
        uint32_t getVertexDataLen() const;
        uint32_t getVertexDataSize() const;
        const uint32_t *getIndicesArray() const;
        uint32_t getIndicesArrayLen() const;
        uint32_t getIndicesArraySize() const;

        /**
         * Method that given a .obj filename loads its contents
         * into a Object3D object that can then be rendered into the scene
         *
         * @param filename  Name of the file with the OBJ data
         *
         * @return true if the object could be loaded, false otherwise
         */
        bool load(const std::string &filename);

    private:
        /**
         * Object data
         */
        std::vector<Object3D::VertexData> _objectData;
        std::vector<uint32_t> _indices;
};
