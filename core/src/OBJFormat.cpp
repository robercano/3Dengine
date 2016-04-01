/**
 * @class OBJLoader
 * @brief OBJ format loader
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#include <stdio.h>
#include <vector>
#include <string>
#include <string.h>
#include <glm/glm.hpp>
#include "OBJFormat.hpp"
#include "ImageLoaders.h"

using namespace std;

bool OBJFormat::load(const string &filename)
{
    std::vector< glm::vec3 > vertices;
    std::vector< glm::vec3 > normals;
    std::vector< glm::vec2 > uvcoords;
    std::map< std::string, Material >::iterator it;
    bool ret = true;
    uint32_t i;
    uint32_t numFaces = 0;
    uint8_t *texture = NULL;
    uint32_t texWidth, texHeight, texBytesPerPixel;
    std::vector<bool> positionSet;
	int t=0;

    std::map< std::string, std::vector<uint32_t> > indices;
    std::map< std::string, Material >              materials;
    std::map< std::string, Texture >               textures;

    std::vector<uint32_t> *activeIndices = NULL;

    std::string matFile = filename + "/material.mtl";
    std::string geoFile = filename + "/geometry.obj";

    /* Open the materials file */
    FILE *file = fopen(matFile.c_str(), "r");
    if (file == NULL)  {
        printf("ERROR cannot open file %s\n", matFile.c_str());
        return false;
    }

    /* Set the default group in case the OBJ file does not
     * contain materials */
    indices["Default"] = std::vector<uint32_t>();
    materials["Default"] = Material();

    for (;;) {
        char line[512];
        int res = fscanf(file, "%[^\r\n]\r\n", line);
        if (res == EOF) {
            break;
        }

        if (strncmp(line, "newmtl ", 7) == 0) {
            std::string matName = line + 7;
            glm::vec3 ambient, diffuse, specular;
            float shininess;

            /* Read the components */
            for (;;) {
                int res = fscanf(file, "%[^\r\n]\r\n", line);
                if (res == EOF) {
                    break;
                }

                /* Ka */
                if (line[0] == 'K' && line[1] == 'a') {
                    res = sscanf(line + 2, "%f %f %f", &ambient.r, &ambient.g, &ambient.b);
                    if (res != 3) {
                        printf("ERROR reading Ka format from OBJ file\n");
                    }
                    continue;
                }
                /* Kd */
                if (line[0] == 'K' && line[1] == 'd') {
                    res = sscanf(line + 2, "%f %f %f", &diffuse.r, &diffuse.g, &diffuse.b);
                    if (res != 3) {
                        printf("ERROR reading Kd format from OBJ file\n");
                    }
                    continue;
                }
                /* Ks */
                if (line[0] == 'K' && line[1] == 's') {
                    res = sscanf(line + 2, "%f %f %f", &specular.r, &specular.g, &specular.b);
                    if (res != 3) {
                        printf("ERROR reading Ks format from OBJ file\n");
                    }
                    continue;
                }
                /* Ns */
                if (line[0] == 'N' && line[1] == 's') {
                    res = sscanf(line + 2, "%f", &shininess);
                    if (res != 1) {
                        printf("ERROR reading Ns format from OBJ file\n");
                    }
                    continue;
                }
                /* map_Kd */
                if (strncmp(line, "map_Kd ", 7) == 0) {
                    std::string texname = filename + std::string("/") + std::string(line+7);

                    if (strcmp(line+7, "null") != 0) {
                        /* Load the texture */
						if (texname.compare(texname.length() - 4, std::string::npos, ".png") == 0) {
							if (loadPNG(texname.c_str(), &texture, &texWidth, &texHeight, &texBytesPerPixel) != 0) {
								printf("ERROR loading PNG texture %s\n", texname.c_str());
								ret = false;
								goto error_exit;
							}
						} else if (texname.compare(texname.length() - 4, std::string::npos, ".jpg") == 0) {
							if (loadJPEG(texname.c_str(), &texture, &texWidth, &texHeight, &texBytesPerPixel) != 0) {
								printf("ERROR loading JPEG texture %s\n", texname.c_str());
								ret = false;
								goto error_exit;
							}
						} else {
							fprintf(stderr, "ERROR texture format not supported\n");
							ret = false;
							goto error_exit;
						}
                    } else {
                        texture = NULL;
                        texWidth = 0;
                        texHeight = 0;
                    }
                    break;
                }

            } /* Material components read */

            /* Add the new material */
            indices[matName] = std::vector<uint32_t>();
            materials[matName] = Material(ambient, diffuse, specular, 1.0, shininess);
            textures[matName]  = Texture(texture, texWidth, texHeight, texBytesPerPixel);

            free(texture);
        }
    }

    fclose(file);

    activeIndices = &indices["Default"];

    /* Open the geometry file */
    file = fopen(geoFile.c_str(), "r");
    if (file == NULL)  {
        printf("ERROR cannot open file %s\n", geoFile.c_str());
        return false;
    }

    for (;;) {
        char line[512];
        int res = fscanf(file, "%[^\r\n]\r\n", line);
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
                printf("ERROR reading vn format from OBJ file\n");
            }
            normals.push_back(normal);
        } else if (line[0] == 'v' && line[1] == 't' && line[2] == ' ') {
            /* Texture coordinates */
            glm::vec2 uv;
            res = sscanf(line + 3, "%f %f", &uv.x, &uv.y);
            if (res != 2) {
                printf("ERROR reading vt format from OBJ file\n");
            }
            /* Adjust uv.y as OBJ defines (0,0) to be top-left corner while
             * OpenGL uses (0,0) for bottom left corner. No idea what DirectX
             * interpretation is. For now just fix it for OpenGL */
            uv.y = 1.0f - uv.y;
            uvcoords.push_back(uv);
        }
    }

    /* Rewind the file */
    fseek(file, SEEK_SET, 0);

    /* Allocate size for the final data */
    _modelData.resize(vertices.size());
    positionSet.resize(vertices.size(), false);

    /* Now parse the groups and the faces */
    for (;;) {
        char line[512];
        int res = fscanf(file, "%[^\r\n]\r\n", line);
        if (res == EOF) {
            break;
        }

        /* Material group */
        if (strncmp(line, "usemtl ", 7) == 0) {
            std::string materialName = line + 7;
            if (materials.find(materialName) == materials.end()) {
                printf("ERROR referenced material %s not found in material list\n", materialName.c_str());
                break;
            }

            activeIndices = &indices[materialName];
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
                uint32_t dataIdx   = vertexIdx;

                if (positionSet[dataIdx] == true) {
                    if (_modelData[dataIdx].normal != normals[normalIdx] ||
                        _modelData[dataIdx].uvcoord != uvcoords[uvIdx]) {

                        dataIdx = _modelData.size();
                        _modelData.resize(_modelData.size() + 1);
                        positionSet.resize(positionSet.size() + 1, false);
                    }
                }

                if (positionSet[dataIdx] == false) {
                    _modelData[dataIdx].vertex  = vertices[vertexIdx];
                    _modelData[dataIdx].normal  = normals[normalIdx];
                    _modelData[dataIdx].uvcoord = uvcoords[uvIdx];

                    positionSet[vertexIdx] = true;
                }

                /* Push the index to the geometry group and
                 * the data to the global data buffer */
                activeIndices->push_back(dataIdx);
            }

            numFaces++;
        }
    }

    /* Now consolidate the data */
    for (it=materials.begin(); it!=materials.end(); ++it) {
        _materials.push_back(it->second);
        _textures.push_back(textures[it->first]);

        std::vector<uint32_t> *idx = &indices[it->first];

        _indicesOffsets.push_back(_modelIndices.size());

        /* Append to the final indices vector */
        _modelIndices.reserve(_modelIndices.size() + idx->size());
        _modelIndices.insert(_modelIndices.end(), idx->begin(), idx->end());

        _indicesCount.push_back(idx->size());
    }

    printf("Loaded %s with %zu vertices and %zu faces\n", filename.c_str(), _modelData.size(), _modelIndices.size()/3);

error_exit:
    fclose(file);
    return ret;
}
