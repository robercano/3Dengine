/**
 * @class OBJLoader
 * @brief OBJ format loader
 *
 * @author Roberto Cano
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
        const float *getVerticesArray() const;
        uint32_t getVerticesArrayLen() const;
        const float *getColorsArray() const;
        uint32_t getColorsArrayLen() const;
        const float *getNormalsArray() const;
        uint32_t getNormalsArrayLen() const;
        const uint32_t *getIndicesArray() const;
        uint32_t getIndicesArrayLen() const;

        /**
         * Static method that given a .obj filename loads its contents
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
        std::vector< glm::vec3 > _vertices;
        std::vector< glm::vec3 > _colors;
        std::vector< glm::vec2 > _uvs;
        std::vector< glm::vec3 > _normals;
        std::vector< uint32_t > _vertexIndices;
        std::vector< uint32_t > _uvIndices;
        std::vector< uint32_t > _normalIndices;
};
