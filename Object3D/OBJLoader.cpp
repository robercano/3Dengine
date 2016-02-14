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
#include "OBJLoader.hpp"

using namespace std;

bool OBJLoader::Load(const string &filename, Object3D *& object)
{
    vector< unsigned int > vertexIndices, uvIndices, normalIndices;
    vector< glm::vec3 > temp_vertices;
    vector< glm::vec2 > temp_uvs;
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
            goto error_exit;
        }

printf("Read line: %s\n", line);

        /* Parse the line */
        if (line[0] == '#' || strlen(line) < 3) {
            continue;
        }

        /* vertices */
        if (line[0] == 'v' && line[1]==' ') {
            glm::vec3 vertex;
            res = sscanf(line + 2, "%f %f %f", &vertex.x, &vertex.y, &vertex.z);
            if (res != 3) {
                printf("ERROR reading v format from OBJ file\n");
            }
            temp_vertices.push_back(vertex);
        } else if (line[0] == 'v' && line[1] == 't' && line[2] == ' ') {
            glm::vec2 uv;
            res = sscanf(line + 3, "%f %f", &uv.x, &uv.y);
            if (res != 2) {
                printf("ERROR reading vt format from OBJ file\n");
            }
            temp_uvs.push_back(uv);
        } else if (line[0] == 'v' && line[1] == 'n' && line[2] == ' ') {
            glm::vec3 normal;
            res = sscanf(line + 3, "%f %f %f", &normal.x, &normal.y, &normal.z);
            if (res != 3) {
                printf("ERROR reading vt format from OBJ file\n");
            }
            temp_normals.push_back(normal);
        } else if (line[0] == 'f' && line[1] == ' ') {
            /* Faces */
            string vertex1, vertex2, vertex3;
            unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
            int matches = sscanf(line + 2, "%d/%d/%d %d/%d/%d %d/%d/%d",
                                 &vertexIndex[0], &uvIndex[0], &normalIndex[0],
                                 &vertexIndex[1], &uvIndex[1], &normalIndex[1],
                                 &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
            if (matches != 9){
                printf("ERROR OBJ file format is not correct for this loader\n");
                ret = false;
                goto error_exit;
            }

            vertexIndices.push_back(vertexIndex[0]);
            vertexIndices.push_back(vertexIndex[1]);
            vertexIndices.push_back(vertexIndex[2]);
            uvIndices    .push_back(uvIndex[0]);
            uvIndices    .push_back(uvIndex[1]);
            uvIndices    .push_back(uvIndex[2]);
            normalIndices.push_back(normalIndex[0]);
            normalIndices.push_back(normalIndex[1]);
            normalIndices.push_back(normalIndex[2]);
        }
    }

error_exit:
    fclose(file);
    return ret;
}
