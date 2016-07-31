/**
 * @class Asset3DStorage
 * @brief Takes care of saving an existing in-memory model to disk and viceversa
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#include "Asset3DStorage.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include "Logging.hpp"
#include "ZCompression.hpp"

using namespace std;
using namespace Logging;

bool Asset3DStorage::Save(const std::string &name, const Asset3D &asset)
{
    ofstream file(name, ios::binary | ios::out | ios::trunc);
    ZCompression comp;

    if (file.is_open() == false) {
        log("ERROR opening file %s\n", name.c_str());
        return false;
    }

    if (comp.init() == false) {
        log("ERROR initializing compression in Asset3DStorage::Save\n");
        file.close();
        return false;
    }

    /* Write the vertex data size */
    uint32_t dataSize = (uint32_t)asset._vertexData.size();
    comp.write(file, (const char *)&dataSize, sizeof dataSize);

    /* Now write the vertex data */
    if (sizeof(Asset3D::VertexData) == Asset3D::VertexDataPackedSize) {
        /* Everything is packed, we can write a single blob */
        comp.write(file, (const char *)&asset._vertexData[0], dataSize * sizeof asset._vertexData[0]);
    } else if (sizeof(Asset3D::VertexData::vertex) + sizeof(Asset3D::VertexData::normal) + sizeof(Asset3D::VertexData::uvcoord) ==
               Asset3D::VertexDataPackedSize) {
        /* vertex, normal and uvcoord are correctly packed internally */
        for (std::vector<Asset3D::VertexData>::const_iterator it = asset._vertexData.begin(); it != asset._vertexData.end(); ++it) {
            comp.write(file, (const char *)&it->vertex, sizeof it->vertex);
            comp.write(file, (const char *)&it->normal, sizeof it->normal);
            comp.write(file, (const char *)&it->uvcoord, sizeof it->uvcoord);
        }
    } else {
        /* Nothing is correctly packed, need to write each coordinate individually */
        for (std::vector<Asset3D::VertexData>::const_iterator it = asset._vertexData.begin(); it != asset._vertexData.end(); ++it) {
            comp.write(file, (const char *)&it->vertex.x, sizeof it->vertex.x);
            comp.write(file, (const char *)&it->vertex.y, sizeof it->vertex.y);
            comp.write(file, (const char *)&it->vertex.z, sizeof it->vertex.z);
            comp.write(file, (const char *)&it->normal.x, sizeof it->normal.x);
            comp.write(file, (const char *)&it->normal.y, sizeof it->normal.y);
            comp.write(file, (const char *)&it->normal.z, sizeof it->normal.z);
            comp.write(file, (const char *)&it->uvcoord.x, sizeof it->uvcoord.x);
            comp.write(file, (const char *)&it->uvcoord.y, sizeof it->uvcoord.y);
        }
    }

    /* Write the materials data size */
    dataSize = (uint32_t)asset._materials.size();
    comp.write(file, (const char *)&dataSize, sizeof dataSize);

    /* Now write the materials data */
    if (sizeof(Material) == Material::PackedSize) {
        /* Everything is packed, we can write a single blob */
        comp.write(file, (const char *)&asset._materials[0], dataSize * sizeof asset._materials[0]);
    } else if (sizeof(Material::_ambient) + sizeof(Material::_diffuse) + sizeof(Material::_specular) + sizeof(Material::_alpha) +
                   sizeof(Material::_shininess) ==
               Material::PackedSize) {
        /* The individual fields are correctly packed internally */
        for (std::vector<Material>::const_iterator it = asset._materials.begin(); it != asset._materials.end(); ++it) {
            comp.write(file, (const char *)&it->_ambient, sizeof it->_ambient);
            comp.write(file, (const char *)&it->_diffuse, sizeof it->_diffuse);
            comp.write(file, (const char *)&it->_specular, sizeof it->_specular);
            comp.write(file, (const char *)&it->_alpha, sizeof it->_alpha);
            comp.write(file, (const char *)&it->_shininess, sizeof it->_shininess);
        }
    } else {
        /* Nothing is correctly packed, need to write each coordinate individually */
        for (std::vector<Material>::const_iterator it = asset._materials.begin(); it != asset._materials.end(); ++it) {
            comp.write(file, (const char *)&it->_ambient.x, sizeof it->_ambient.x);
            comp.write(file, (const char *)&it->_ambient.y, sizeof it->_ambient.y);
            comp.write(file, (const char *)&it->_ambient.z, sizeof it->_ambient.z);
            comp.write(file, (const char *)&it->_diffuse.x, sizeof it->_diffuse.x);
            comp.write(file, (const char *)&it->_diffuse.y, sizeof it->_diffuse.y);
            comp.write(file, (const char *)&it->_diffuse.z, sizeof it->_diffuse.z);
            comp.write(file, (const char *)&it->_specular.x, sizeof it->_specular.x);
            comp.write(file, (const char *)&it->_specular.y, sizeof it->_specular.y);
            comp.write(file, (const char *)&it->_specular.z, sizeof it->_specular.z);
            comp.write(file, (const char *)&it->_alpha, sizeof it->_alpha);
            comp.write(file, (const char *)&it->_shininess, sizeof it->_shininess);
        }
    }

    /* Write the textures data size */
    dataSize = (uint32_t)asset._textures.size();
    comp.write(file, (const char *)&dataSize, sizeof dataSize);

    /* Now write the texture data */
    for (std::vector<Texture>::const_iterator it = asset._textures.begin(); it != asset._textures.end(); ++it) {
        comp.write(file, (const char *)&it->_width, sizeof it->_width);
        comp.write(file, (const char *)&it->_height, sizeof it->_height);
        comp.write(file, (const char *)&it->_Bpp, sizeof it->_Bpp);
        comp.write(file, (const char *)it->_texture, it->_width * it->_height * it->_Bpp);
    }

    /* Write the indices data size */
    dataSize = (uint32_t)asset._vertexIndices.size();
    comp.write(file, (const char *)&dataSize, sizeof dataSize);

    /* Now write the indices data */
    comp.write(file, (const char *)&asset._vertexIndices[0], dataSize * sizeof asset._vertexIndices[0]);

    /* Write the indices offsets data size */
    dataSize = (uint32_t)asset._indicesOffsets.size();
    comp.write(file, (const char *)&dataSize, sizeof dataSize);

    /* Now write the indices offsets data */
    comp.write(file, (const char *)&asset._indicesOffsets[0], dataSize * sizeof asset._indicesOffsets[0]);

    /* Write the indices count data size */
    dataSize = (uint32_t)asset._indicesCount.size();
    comp.write(file, (const char *)&dataSize, sizeof dataSize);

    /* Now write the indices count data */
    comp.write(file, (const char *)&asset._indicesCount[0], dataSize * sizeof asset._indicesCount[0], true);

    comp.finish();

    if (file.bad() == true) {
        log("ERROR writing data to file %s\n", name.c_str());
        file.close();
        return false;
    }

    /* Close the file */
    file.close();

    return true;
}

bool Asset3DStorage::Load(const std::string &name, Asset3D &asset)
{
    uint32_t dataSize;
    ifstream file(name, ios::binary | ios::in);
    ZDecompression dcomp;

    if (file.is_open() == false) {
        log("ERROR opening file %s\n", name.c_str());
        return false;
    }

    if (dcomp.init() == false) {
        log("ERROR initializing decompression in Asset3DStorage::Load\n");
        file.close();
        return false;
    }

    /* Read the vertex data size */
    dcomp.read(file, (char *)&dataSize, sizeof dataSize);
    asset._vertexData.resize(dataSize);

    /* Now read the vertex data */
    if (sizeof(Asset3D::VertexData) == Asset3D::VertexDataPackedSize) {
        /* Everything is packed, we can read a single blob */
        dcomp.read(file, (char *)&asset._vertexData[0], dataSize * sizeof asset._vertexData[0]);
    } else if (sizeof(Asset3D::VertexData::vertex) + sizeof(Asset3D::VertexData::normal) + sizeof(Asset3D::VertexData::uvcoord) ==
               Asset3D::VertexDataPackedSize) {
        /* vertex, normal and uvcoord are correctly packed internally */
        for (std::vector<Asset3D::VertexData>::iterator it = asset._vertexData.begin(); it != asset._vertexData.end(); ++it) {
            dcomp.read(file, (char *)&it->vertex, sizeof it->vertex);
            dcomp.read(file, (char *)&it->normal, sizeof it->normal);
            dcomp.read(file, (char *)&it->uvcoord, sizeof it->uvcoord);
        }
    } else {
        /* Nothing is correctly packed, need to read each coordinate individually */
        for (std::vector<Asset3D::VertexData>::iterator it = asset._vertexData.begin(); it != asset._vertexData.end(); ++it) {
            dcomp.read(file, (char *)&it->vertex.x, sizeof it->vertex.x);
            dcomp.read(file, (char *)&it->vertex.y, sizeof it->vertex.y);
            dcomp.read(file, (char *)&it->vertex.z, sizeof it->vertex.z);
            dcomp.read(file, (char *)&it->normal.x, sizeof it->normal.x);
            dcomp.read(file, (char *)&it->normal.y, sizeof it->normal.y);
            dcomp.read(file, (char *)&it->normal.z, sizeof it->normal.z);
            dcomp.read(file, (char *)&it->uvcoord.x, sizeof it->uvcoord.x);
            dcomp.read(file, (char *)&it->uvcoord.y, sizeof it->uvcoord.y);
        }
    }

    /* Read the materials data size */
    dcomp.read(file, (char *)&dataSize, sizeof dataSize);
    asset._materials.resize(dataSize);

    /* Now read the materials data */
    if (sizeof(Material) == Material::PackedSize) {
        /* Everything is packed, we can read a single blob */
        dcomp.read(file, (char *)&asset._materials[0], dataSize * sizeof asset._materials[0]);
    } else if (sizeof(Material::_ambient) + sizeof(Material::_diffuse) + sizeof(Material::_specular) + sizeof(Material::_alpha) +
                   sizeof(Material::_shininess) ==
               Material::PackedSize) {
        /* The individual fields are correctly packed internally */
        for (std::vector<Material>::iterator it = asset._materials.begin(); it != asset._materials.end(); ++it) {
            dcomp.read(file, (char *)&it->_ambient, sizeof it->_ambient);
            dcomp.read(file, (char *)&it->_diffuse, sizeof it->_diffuse);
            dcomp.read(file, (char *)&it->_specular, sizeof it->_specular);
            dcomp.read(file, (char *)&it->_alpha, sizeof it->_alpha);
            dcomp.read(file, (char *)&it->_shininess, sizeof it->_shininess);
        }
    } else {
        /* Nothing is correctly packed, need to read each coordinate individually */
        for (std::vector<Material>::iterator it = asset._materials.begin(); it != asset._materials.end(); ++it) {
            dcomp.read(file, (char *)&it->_ambient.x, sizeof it->_ambient.x);
            dcomp.read(file, (char *)&it->_ambient.y, sizeof it->_ambient.y);
            dcomp.read(file, (char *)&it->_ambient.z, sizeof it->_ambient.z);
            dcomp.read(file, (char *)&it->_diffuse.x, sizeof it->_diffuse.x);
            dcomp.read(file, (char *)&it->_diffuse.y, sizeof it->_diffuse.y);
            dcomp.read(file, (char *)&it->_diffuse.z, sizeof it->_diffuse.z);
            dcomp.read(file, (char *)&it->_specular.x, sizeof it->_specular.x);
            dcomp.read(file, (char *)&it->_specular.y, sizeof it->_specular.y);
            dcomp.read(file, (char *)&it->_specular.z, sizeof it->_specular.z);
            dcomp.read(file, (char *)&it->_alpha, sizeof it->_alpha);
            dcomp.read(file, (char *)&it->_shininess, sizeof it->_shininess);
        }
    }

    /* Read the textures data size */
    dcomp.read(file, (char *)&dataSize, sizeof dataSize);
    asset._textures.resize(dataSize);

    /* Now read the texture data */
    for (std::vector<Texture>::iterator it = asset._textures.begin(); it != asset._textures.end(); ++it) {
        dcomp.read(file, (char *)&it->_width, sizeof it->_width);
        dcomp.read(file, (char *)&it->_height, sizeof it->_height);
        dcomp.read(file, (char *)&it->_Bpp, sizeof it->_Bpp);
        it->_texture = new uint8_t[it->_width * it->_height * it->_Bpp];
        dcomp.read(file, (char *)it->_texture, it->_width * it->_height * it->_Bpp);
    }

    /* Read the indices data size */
    dcomp.read(file, (char *)&dataSize, sizeof dataSize);
    asset._vertexIndices.resize(dataSize);

    /* Now read the indices data */
    dcomp.read(file, (char *)&asset._vertexIndices[0], dataSize * sizeof asset._vertexIndices[0]);

    /* Read the indices offsets data size */
    dcomp.read(file, (char *)&dataSize, sizeof dataSize);
    asset._indicesOffsets.resize(dataSize);

    /* Now read the indices offsets data */
    dcomp.read(file, (char *)&asset._indicesOffsets[0], dataSize * sizeof asset._indicesOffsets[0]);

    /* Read the indices count data size */
    dcomp.read(file, (char *)&dataSize, sizeof dataSize);
    asset._indicesCount.resize(dataSize);

    /* Now read the indices count data */
    dcomp.read(file, (char *)&asset._indicesCount[0], dataSize * sizeof asset._indicesCount[0]);

    if (file.bad() == true) {
        log("ERROR reading data from file %s\n", name.c_str());
        file.close();
        return false;
    }

    /* Close the file */
    file.close();

    return true;
}
