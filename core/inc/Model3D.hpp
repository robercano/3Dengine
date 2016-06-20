/**
 * @class	Model3D
 * @brief	A Model3D represents a 3D asset in the engine including the geometry,
 *          normals, textures and texture coordinates, and vertices indices for
 *          indexed geometry rendering. This class can be expanded to support any
 *          3D model definition that can be needed in the engine, like displacement
 *          maps. This class represents an static model and does not take into
 *          account animations or tessellation of skin models.
 *
 *          Objects of this class cannot be instantiated directly and cannot only
 *          be obtained through model loaders.
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
#include "Object3D.hpp"
#include "Texture.hpp"

class Model3D : public Object3D
{
  public:
    /**
     * Vertex data of the model
     */
    typedef struct {
        glm::vec3 vertex;  /**< Vertex in model coordinates */
        glm::vec3 normal;  /**< Normal of the vertex */
        glm::vec2 uvcoord; /**< Textures coordinates of the vertex */
    } VertexData;

    /**
     * Destructor
     */
    virtual ~Model3D() {}

    /**
     * Normalizes all vertex data in the Model3D
     *
     * This method calculates the center of mass of all vertices in the model
     * and then normalizes all vertices with respect to this CoM so the maximum length
     * from a vertex position to the CoM is 1.0f. This is typically used after loading
     * a model from disk which hasn't been imported before in the engine so the user can
     * make sure the model will be visible on screen
     */
	void normalize();

    /**
     * Getter
     *
     * All getter return a reference to one of the internal vectors containing
     * the model definition for a specific subset of the data. The data is layed
     * out consecutively on memory so the caller can obtain the pointer to the
     * first element and load the buffer into the GPU
     */
    const std::vector<Model3D::VertexData> &getVertexData() const { return _modelData; }
    const std::vector<uint32_t> &getIndexData() const { return _modelIndices; }
    const std::vector<Material> &getMaterials() const { return _materials; }
    const std::vector<Texture> &getTextures() const { return _textures; }
    const std::vector<uint32_t> &getIndicesOffsets() const { return _indicesOffsets; }
    const std::vector<uint32_t> &getIndicesCount() const { return _indicesCount; }

  protected:
    /**
     * Constructor
     */
    Model3D() {}

	/**
	 * Methods from Object3D class
	 */
	void _calculateBoundingVolumes();

    std::vector<Model3D::VertexData> _modelData; /**< Data containing the vertex position, normal and UV coordinates */
    std::vector<Material> _materials;            /**< List of materials used in the model */
    std::vector<Texture> _textures;              /**< List of textures used in the model */
    std::vector<uint32_t> _modelIndices;         /**< List of indices containing all rendering lists together */
    std::vector<uint32_t> _indicesOffsets;       /**< Offset in _modelIndices of the beginning of the rendering list number 'n' */
    std::vector<uint32_t> _indicesCount;         /**< Number of indices belonging to the rendering list number 'n' */
};
