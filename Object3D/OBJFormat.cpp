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
    vector< glm::vec3 > temp_normals;
    bool ret = true;
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
            glm::vec3 color;
            res = sscanf(line + 2, "%f %f %f", &vertex.x, &vertex.y, &vertex.z);
            if (res != 3) {
                printf("ERROR reading v format from OBJ file\n");
            }
            _vertices.push_back(vertex);
/* HACK TODO! */
color = glm::normalize(vertex);
_colors.push_back(color);
        } else if (line[0] == 'v' && line[1] == 't' && line[2] == ' ') {
            /* Texture coordinates */
            glm::vec2 uv;
            res = sscanf(line + 3, "%f %f", &uv.x, &uv.y);
            if (res != 2) {
                printf("ERROR reading vt format from OBJ file\n");
            }
            _uvs.push_back(uv);
        } else if (line[0] == 'v' && line[1] == 'n' && line[2] == ' ') {
            /* Normals */
            glm::vec3 normal;
            res = sscanf(line + 3, "%f %f %f", &normal.x, &normal.y, &normal.z);
            if (res != 3) {
                printf("ERROR reading vt format from OBJ file\n");
            }
            temp_normals.push_back(normal);
        }
    }

    /* Rewind the file */
    fseek(file, SEEK_SET, 0);

    /* Resize the vector for the normals */
    _normals.resize(_vertices.size());

    /* Now parse the faces */
    for (;;) {
        char line[512];
        int res = fscanf(file, "%[^\n]\n", line);
        if (res == EOF) {
            break;
        }

        /* Faces */
        if (line[0] == 'f' && line[1] == ' ') {
            string vertex1, vertex2, vertex3;
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

            _vertexIndices.push_back(vertexIndex[0] - 1);
            _vertexIndices.push_back(vertexIndex[1] - 1);
            _vertexIndices.push_back(vertexIndex[2] - 1);
            _normals[vertexIndex[0] - 1] = temp_normals[normalIndex[0] - 1];
            _normals[vertexIndex[1] - 1] = temp_normals[normalIndex[1] - 1];
            _normals[vertexIndex[2] - 1] = temp_normals[normalIndex[2] - 1];

            _uvIndices    .push_back(uvIndex[0]);
            _uvIndices    .push_back(uvIndex[1]);
            _uvIndices    .push_back(uvIndex[2]);
            _normalIndices.push_back(normalIndex[0]);
            _normalIndices.push_back(normalIndex[1]);
            _normalIndices.push_back(normalIndex[2]);
        }
    }

error_exit:
    fclose(file);
    return ret;
}

const GLfloat *OBJFormat::getVerticesArray()
{
    return (GLfloat*)&_vertices[0];
}

uint32_t OBJFormat::getVerticesArrayLen()
{
    return _vertices.size() * sizeof(glm::vec3);
}

const GLfloat *OBJFormat::getColorsArray()
{
    return (GLfloat*)&_colors[0];
}

uint32_t OBJFormat::getColorsArrayLen()
{
    return _colors.size() * sizeof(glm::vec3);
}

const GLfloat *OBJFormat::getNormalsArray()
{
    return (GLfloat*)&_normals[0];
}

uint32_t OBJFormat::getNormalsArrayLen()
{
    return _normals.size() * sizeof(glm::vec3);
}

const GLuint *OBJFormat::getIndicesArray()
{
    return (GLuint*)&_vertexIndices[0];
}

uint32_t OBJFormat::getIndicesArrayLen()
{
    return _vertexIndices.size();
}
