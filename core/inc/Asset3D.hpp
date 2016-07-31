/**
 * @class	Model3D
 * @brief	An Asset3D object represents a 3D asset in the engine including the geometry,
 *          normals, textures and texture coordinates, and vertices indices for
 *          indexed geometry rendering. This class can be expanded to support any
 *          3D model definition that can be needed in the engine, like displacement
 *          maps. This class represents an static model and does not take into
 *          account animations or tessellation of skin models.
 *
 *          Objects of this class cannot be instantiated directly and can only
 *          be obtained through the model loaders of the Renderer class.
 *
 *          The data of the 3D model is organized as follows:
 *
 *              * List of vertices of the model including position, normal and UV coordinate.
 *                If a vertex belongs to 2 different polygons with different textures or surface
 *                directions, the same vertex position can appear more than once with different
 *                normal and/or different UV coordinate
 *              * List of indices for triangle rendering lists. The list of indices is always
 *                a multiple of 3 and it specifies how to group 3 vertices to form a triangle,
 *                including the face direction (counter clock-wise)
 *              * The following vector define the rendering lists. Each entry of the folowing vectors
 *                define 1 display list of triangles with it's corresponding material and texture. All
 *                the following vectors must have the same length:
 *
 *                  * List of textures
 *                  * List of materials
 *                  * List of offsets into the indices list for the start of the rendering list
 *                  * List with the number of indices that belong to the rendering list starting
 *                    at the indicated offset
 *
 *          The former data allows to render a model3D as a group of rendering lists, with each list
 *          defining a material and a texture, and a list of indices into the raw vertex data. This
 *          minimizes the bandwidth required from the GPU to render the model.
 *
 *          TODO: This class is likely to be refactored when animations are introduced
 *          in the engine
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include <stdint.h>
#include <glm/glm.hpp>
#include <vector>
#include "Material.hpp"
#include "Texture.hpp"
#include "ProceduralUtils.hpp"

class Asset3D
{
  public:
    /**
     * Friend classes
     */
    friend class Asset3DStorage;
    friend class Asset3DLoaders;
    friend class Asset3DTransform;
    friend void Procedural::AppendBentPlane(Asset3D &asset,
                                            float width, float height,
                                            float angleWidth, float angleHeight, float angleRadius,
                                            uint32_t numVertsWidth, uint32_t numVertsHeight);

    /**
     * Vertex data of the model
     */
    struct VertexData {
        glm::vec3 vertex;  /**< Vertex in model coordinates */
        glm::vec3 normal;  /**< Normal of the vertex */
        glm::vec2 uvcoord; /**< Textures coordinates of the vertex */
    };

    /**
     * This constant defines the expected size of the VertexData structure
     * if it is correctly packed. It is used to optimize the serialization
     * process of the model's vertex data
     */
    static const uint32_t VertexDataPackedSize = 32;

    /**
     * Constructor
     */
    Asset3D() {}

    /**
     * Destructor
     */
    virtual ~Asset3D() {}

    /**
     * Normalizes all vertex data in the Asset3D
     *
     * This method calculates the center of mass of all vertices in the model
     * and then normalizes all vertices with respect to this CoM so the maximum length
     * from a vertex position to the CoM is 1.0f. This is typically used after loading
     * a model from disk which hasn't been imported before in the engine so the user can
     * make sure the model will be visible on screen
     */
    void normalize()
    {
        std::vector<VertexData>::iterator it;
        glm::vec3 cm = glm::vec3(0.0f, 0.0f, 0.0f);

        for (it = _vertexData.begin(); it != _vertexData.end(); ++it) {
            cm += it->vertex;
        }
        cm /= _vertexData.size();

        /* Substract the center of mass to all vertices */
        float maxLength = std::numeric_limits<float>::min();

        for (it = _vertexData.begin(); it != _vertexData.end(); ++it) {
            it->vertex -= cm;

            /* Calculate maximum length */
            float length = glm::length(it->vertex);
            if (length > maxLength) {
                maxLength = length;
            }
        }

        /* Finally divide by maxLength to make the model fit in a sphere
         * of radius 1.0 */
        for (it = _vertexData.begin(); it != _vertexData.end(); ++it) {
            it->vertex /= maxLength;
        }
    }

    /**
     * Getter
     *
     * All getter return a reference to one of the internal vectors containing
     * the model definition for a specific subset of the data. The data is layed
     * out consecutively on memory so the caller can obtain the pointer to the
     * first element and load the buffer into the GPU
     */
    const std::vector<Asset3D::VertexData> &getVertexData() const { return _vertexData; }
    const std::vector<uint32_t> &getIndexData() const { return _vertexIndices; }
    const std::vector<Material> &getMaterials() const { return _materials; }
    const std::vector<Texture> &getTextures() const { return _textures; }
    const std::vector<uint32_t> &getIndicesOffsets() const { return _indicesOffsets; }
    const std::vector<uint32_t> &getIndicesCount() const { return _indicesCount; }

  protected:
    std::vector<Asset3D::VertexData> _vertexData; /**< Data containing the vertex position, normal and UV coordinates */
    std::vector<Material> _materials;            /**< List of materials used in the model */
    std::vector<Texture> _textures;              /**< List of textures used in the model */
    std::vector<uint32_t> _vertexIndices;        /**< List of indices containing all rendering lists together */
    std::vector<uint32_t> _indicesOffsets;       /**< Offset in _vertexIndices of the beginning of the rendering list number 'n' */
    std::vector<uint32_t> _indicesCount;         /**< Number of indices belonging to the rendering list number 'n' */
};
