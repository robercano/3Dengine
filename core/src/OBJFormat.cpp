/**
 * @class OBJLoader
 * @brief OBJ format loader
 *
 * @author Roberto Cano
 */
#include <stdio.h>
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include "OBJFormat.hpp"

using namespace std;

bool OBJFormat::load(const string &filename)
{
    std::vector< glm::vec3 > vertices;
    std::vector< glm::vec3 > normals;
    std::vector< glm::vec2 > uvcoords;
    bool ret = true;
    uint32_t i;

    FILE *file = fopen(filename.c_str(), "r");
    if (file == NULL)  {
        printf("ERROR cannot open file %s\n", filename.c_str());
        return false;
    }

    for (;;) {
        char line[512];
        int res = fscanf(file, "%[^\n]\n", line);
        if (res == EOF) {
            break;
        }

        /* vertices */
        if (line[0] == 'v' && line[1]==' ') {
            glm::vec3 vertex;
            res = sscanf(line + 2, "%f %f %f", &vertex.x, &vertex.y, &vertex.z);
            if (res != 3) {
                printf("ERROR reading v format from OBJ file\n");
            }
            vertices.push_back(vertex);
        } else if (line[0] == 'v' && line[1] == 'n' && line[2] == ' ') {
            /* Normals */
            glm::vec3 normal;
            res = sscanf(line + 3, "%f %f %f", &normal.x, &normal.y, &normal.z);
            if (res != 3) {
                printf("ERROR reading vt format from OBJ file\n");
            }
            normals.push_back(normal);
        } else if (line[0] == 'v' && line[1] == 't' && line[2] == ' ') {
            /* Texture coordinates */
            glm::vec2 uv;
            res = sscanf(line + 3, "%f %f", &uv.x, &uv.y);
            if (res != 2) {
                printf("ERROR reading vt format from OBJ file\n");
            }
            uvcoords.push_back(uv);
        }
    }

    /* Rewind the file */
    fseek(file, SEEK_SET, 0);

    /* Allocate size for the final data */
    _objectData.resize(vertices.size());

    /* Now parse the faces */
    for (;;) {
        char line[512];
        int res = fscanf(file, "%[^\n]\n", line);
        if (res == EOF) {
            break;
        }

        /* Faces */
        if (line[0] == 'f' && line[1] == ' ') {
            unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
            int matches = sscanf(line + 2, "%d/%d/%d %d/%d/%d %d/%d/%d",
                                 &vertexIndex[0], &uvIndex[0], &normalIndex[0],
                                 &vertexIndex[1], &uvIndex[1], &normalIndex[1],
                                 &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
            if (matches != 9){
                printf("ERROR OBJ file format is not correct for this loader %d\n", matches);
                ret = false;
                goto error_exit;
            }

            /* Fill the indices for each triangle */
            for (i=0; i<3; ++i) {
                uint32_t vertexIdx = vertexIndex[i] - 1;
                uint32_t normalIdx = normalIndex[i] - 1;
                uint32_t uvIdx     = uvIndex[i] - 1;

                _indices.push_back(vertexIdx);

                _objectData[vertexIdx].vertex  = vertices[vertexIdx];
                _objectData[vertexIdx].normal  = normals[normalIdx];
                _objectData[vertexIdx].uvcoord = uvcoords[uvIdx];
            }
        }
    }

    printf("Loaded %s with %zu vertices and %zu faces\n", filename.c_str(), _objectData.size(), _indices.size()/3);

error_exit:
    fclose(file);
    return ret;
}

const Object3D::VertexData *OBJFormat::getVertexData() const
{
    return (Object3D::VertexData*)&_objectData[0];
}

uint32_t OBJFormat::getVertexDataLen() const
{
    return _objectData.size();
}

uint32_t OBJFormat::getVertexDataSize() const
{
    return _objectData.size() * sizeof(Object3D::VertexData);
}

const uint32_t *OBJFormat::getIndicesArray() const
{
    return (uint32_t*)&_indices[0];
}

uint32_t OBJFormat::getIndicesArrayLen() const
{
    return _indices.size();
}

uint32_t OBJFormat::getIndicesArraySize() const
{
    return _indices.size() * sizeof(uint32_t);
}
