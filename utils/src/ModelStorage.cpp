/**
 * @class ModelStorage
 * @brief Takes care of saving an existing in-memory model to disk and viceversa
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#include "ModelStorage.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include "Logging.hpp"

using namespace std;
using namespace Logging;

bool ModelStorage::Save(const std::string &name, const Model3D &model)
{
    ofstream file(name, ios::binary | ios::out | ios::trunc);

    if (file.is_open() == false) {
        log("ERROR opening file %s\n", name.c_str());
        return false;
    }

    /* Write the vertex data size */
    uint32_t dataSize = (uint32_t)model._modelData.size();
    file.write((const char *)&dataSize, sizeof dataSize);

    /* Now write the vertex data */
    if (sizeof(Model3D::VertexData) == Model3D::VertexDataPackedSize) {
        /* Everything is packed, we can write a single blob */
        file.write((const char *)&model._modelData[0], dataSize * sizeof model._modelData[0]);
    } else if (sizeof(Model3D::VertexData::vertex) + sizeof(Model3D::VertexData::normal) + sizeof(Model3D::VertexData::uvcoord) ==
               Model3D::VertexDataPackedSize) {
        /* vertex, normal and uvcoord are correctly packed internally */
        for (std::vector<Model3D::VertexData>::const_iterator it = model._modelData.begin(); it != model._modelData.end(); ++it) {
            file.write((const char *)&it->vertex, sizeof it->vertex);
            file.write((const char *)&it->normal, sizeof it->normal);
            file.write((const char *)&it->uvcoord, sizeof it->uvcoord);
        }
    } else {
        /* Nothing is correctly packed, need to write each coordinate individually */
        for (std::vector<Model3D::VertexData>::const_iterator it = model._modelData.begin(); it != model._modelData.end(); ++it) {
            file.write((const char *)&it->vertex.x, sizeof it->vertex.x);
            file.write((const char *)&it->vertex.y, sizeof it->vertex.y);
            file.write((const char *)&it->vertex.z, sizeof it->vertex.z);
            file.write((const char *)&it->normal.x, sizeof it->normal.x);
            file.write((const char *)&it->normal.y, sizeof it->normal.y);
            file.write((const char *)&it->normal.z, sizeof it->normal.z);
            file.write((const char *)&it->uvcoord.x, sizeof it->uvcoord.x);
            file.write((const char *)&it->uvcoord.y, sizeof it->uvcoord.y);
        }
    }

    /* Write the materials data size */
    dataSize = (uint32_t)model._materials.size();
    file.write((const char *)&dataSize, sizeof dataSize);

    /* Now write the materials data */
    if (sizeof(Material) == Material::PackedSize) {
        /* Everything is packed, we can write a single blob */
        file.write((const char *)&model._materials[0], dataSize * sizeof model._modelData[0]);
    } else if (sizeof(Material::_ambient) + sizeof(Material::_diffuse) + sizeof(Material::_specular) + sizeof(Material::_alpha) +
                   sizeof(Material::_shininess) ==
               Material::PackedSize) {
        /* The individual fields are correctly packed internally */
        for (std::vector<Material>::const_iterator it = model._materials.begin(); it != model._materials.end(); ++it) {
            file.write((const char *)&it->_ambient, sizeof it->_ambient);
            file.write((const char *)&it->_diffuse, sizeof it->_diffuse);
            file.write((const char *)&it->_specular, sizeof it->_specular);
            file.write((const char *)&it->_alpha, sizeof it->_alpha);
            file.write((const char *)&it->_shininess, sizeof it->_shininess);
        }
    } else {
        /* Nothing is correctly packed, need to write each coordinate individually */
        for (std::vector<Material>::const_iterator it = model._materials.begin(); it != model._materials.end(); ++it) {
            file.write((const char *)&it->_ambient.x, sizeof it->_ambient.x);
            file.write((const char *)&it->_ambient.y, sizeof it->_ambient.y);
            file.write((const char *)&it->_ambient.z, sizeof it->_ambient.z);
            file.write((const char *)&it->_diffuse.x, sizeof it->_diffuse.x);
            file.write((const char *)&it->_diffuse.y, sizeof it->_diffuse.y);
            file.write((const char *)&it->_diffuse.z, sizeof it->_diffuse.z);
            file.write((const char *)&it->_specular.x, sizeof it->_specular.x);
            file.write((const char *)&it->_specular.y, sizeof it->_specular.y);
            file.write((const char *)&it->_specular.z, sizeof it->_specular.z);
            file.write((const char *)&it->_alpha, sizeof it->_alpha);
            file.write((const char *)&it->_shininess, sizeof it->_shininess);
        }
    }

    /* Write the textures data size */
    dataSize = (uint32_t)model._textures.size();
    file.write((const char *)&dataSize, sizeof dataSize);

    /* Now write the texture data */
    for (std::vector<Texture>::const_iterator it = model._textures.begin(); it != model._textures.end(); ++it) {
        file.write((const char *)it->_texture, it->_width * it->_height * it->_Bpp);
        file.write((const char *)&it->_width, sizeof it->_width);
        file.write((const char *)&it->_height, sizeof it->_height);
        file.write((const char *)&it->_Bpp, sizeof it->_Bpp);
    }

    /* Write the indices data size */
    dataSize = (uint32_t)model._modelIndices.size();
    file.write((const char *)&dataSize, sizeof dataSize);

    /* Now write the indices data */
    file.write((const char *)&model._modelIndices[0], dataSize * sizeof model._modelIndices[0]);

    /* Write the indices offsets data size */
    dataSize = (uint32_t)model._indicesOffsets.size();
    file.write((const char *)&dataSize, sizeof dataSize);

    /* Now write the indices offsets data */
    file.write((const char *)&model._indicesOffsets[0], dataSize * sizeof model._indicesOffsets[0]);

    /* Write the indices count data size */
    dataSize = (uint32_t)model._indicesCount.size();
    file.write((const char *)&dataSize, sizeof dataSize);

    /* Now write the indices count data */
    file.write((const char *)&model._indicesCount[0], dataSize * sizeof model._indicesCount[0]);

    if (file.bad() == true) {
        log("ERROR writing data to file %s\n", name.c_str());
        file.close();
        return false;
    }

    /* Close the file */
    file.close();

    return true;
}

bool ModelStorage::Load(const std::string &name, Model3D &model)
{
    uint32_t dataSize;
    ifstream file(name, ios::binary | ios::in);

    if (file.is_open() == false) {
        log("ERROR opening file %s\n", name.c_str());
        return false;
    }

    /* Read the vertex data size */
    file.read((char *)&dataSize, sizeof dataSize);
    model._modelData.resize(dataSize);

    /* Now read the vertex data */
    if (sizeof(Model3D::VertexData) == Model3D::VertexDataPackedSize) {
        /* Everything is packed, we can read a single blob */
        file.read((char *)&model._modelData[0], dataSize * sizeof model._modelData[0]);
    } else if (sizeof(Model3D::VertexData::vertex) + sizeof(Model3D::VertexData::normal) + sizeof(Model3D::VertexData::uvcoord) ==
               Model3D::VertexDataPackedSize) {
        /* vertex, normal and uvcoord are correctly packed internally */
        for (std::vector<Model3D::VertexData>::const_iterator it = model._modelData.begin(); it != model._modelData.end(); ++it) {
            file.read((char *)&it->vertex, sizeof it->vertex);
            file.read((char *)&it->normal, sizeof it->normal);
            file.read((char *)&it->uvcoord, sizeof it->uvcoord);
        }
    } else {
        /* Nothing is correctly packed, need to read each coordinate individually */
        for (std::vector<Model3D::VertexData>::const_iterator it = model._modelData.begin(); it != model._modelData.end(); ++it) {
            file.read((char *)&it->vertex.x, sizeof it->vertex.x);
            file.read((char *)&it->vertex.y, sizeof it->vertex.y);
            file.read((char *)&it->vertex.z, sizeof it->vertex.z);
            file.read((char *)&it->normal.x, sizeof it->normal.x);
            file.read((char *)&it->normal.y, sizeof it->normal.y);
            file.read((char *)&it->normal.z, sizeof it->normal.z);
            file.read((char *)&it->uvcoord.x, sizeof it->uvcoord.x);
            file.read((char *)&it->uvcoord.y, sizeof it->uvcoord.y);
        }
    }

    /* Read the materials data size */
    file.read((char *)&dataSize, sizeof dataSize);
    model._materials.resize(dataSize);

    /* Now read the materials data */
    if (sizeof(Material) == Material::PackedSize) {
        /* Everything is packed, we can read a single blob */
        file.read((char *)&model._materials[0], dataSize * sizeof model._modelData[0]);
    } else if (sizeof(Material::_ambient) + sizeof(Material::_diffuse) + sizeof(Material::_specular) + sizeof(Material::_alpha) +
                   sizeof(Material::_shininess) ==
               Material::PackedSize) {
        /* The individual fields are correctly packed internally */
        for (std::vector<Material>::const_iterator it = model._materials.begin(); it != model._materials.end(); ++it) {
            file.read((char *)&it->_ambient, sizeof it->_ambient);
            file.read((char *)&it->_diffuse, sizeof it->_diffuse);
            file.read((char *)&it->_specular, sizeof it->_specular);
            file.read((char *)&it->_alpha, sizeof it->_alpha);
            file.read((char *)&it->_shininess, sizeof it->_shininess);
        }
    } else {
        /* Nothing is correctly packed, need to read each coordinate individually */
        for (std::vector<Material>::const_iterator it = model._materials.begin(); it != model._materials.end(); ++it) {
            file.read((char *)&it->_ambient.x, sizeof it->_ambient.x);
            file.read((char *)&it->_ambient.y, sizeof it->_ambient.y);
            file.read((char *)&it->_ambient.z, sizeof it->_ambient.z);
            file.read((char *)&it->_diffuse.x, sizeof it->_diffuse.x);
            file.read((char *)&it->_diffuse.y, sizeof it->_diffuse.y);
            file.read((char *)&it->_diffuse.z, sizeof it->_diffuse.z);
            file.read((char *)&it->_specular.x, sizeof it->_specular.x);
            file.read((char *)&it->_specular.y, sizeof it->_specular.y);
            file.read((char *)&it->_specular.z, sizeof it->_specular.z);
            file.read((char *)&it->_alpha, sizeof it->_alpha);
            file.read((char *)&it->_shininess, sizeof it->_shininess);
        }
    }

    /* Read the textures data size */
    file.read((char *)&dataSize, sizeof dataSize);

    model._textures.resize(dataSize);

    /* Now read the texture data */
    for (std::vector<Texture>::const_iterator it = model._textures.begin(); it != model._textures.end(); ++it) {
        file.read((char *)it->_texture, it->_width * it->_height * it->_Bpp);
        file.read((char *)&it->_width, sizeof it->_width);
        file.read((char *)&it->_height, sizeof it->_height);
        file.read((char *)&it->_Bpp, sizeof it->_Bpp);
    }

    /* Read the indices data size */
    file.read((char *)&dataSize, sizeof dataSize);
    model._modelIndices.resize(dataSize);

    /* Now read the indices data */
    file.read((char *)&model._modelIndices[0], dataSize * sizeof model._modelIndices[0]);

    /* Read the indices offsets data size */
    file.read((char *)&dataSize, sizeof dataSize);
    model._indicesOffsets.resize(dataSize);

    /* Now read the indices offsets data */
    file.read((char *)&model._indicesOffsets[0], dataSize * sizeof model._indicesOffsets[0]);

    /* Read the indices count data size */
    file.read((char *)&dataSize, sizeof dataSize);
    model._indicesCount.resize(dataSize);

    /* Now read the indices count data */
    file.read((char *)&model._indicesCount[0], dataSize * sizeof model._indicesCount[0]);

    if (file.bad() == true) {
        log("ERROR reading data from file %s\n", name.c_str());
        file.close();
        return false;
    }

    /* Close the file */
    file.close();

    return true;
}
