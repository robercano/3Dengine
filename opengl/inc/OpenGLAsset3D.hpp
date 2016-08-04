/**
 * @class	OpenGLAsset3D
 * @brief	Asset3D representation prepared for the specific renderer
 *          This is a composite model generated through Renderer::prepareAsset3D()
 *          that prepares the asset for the specific renderer. For example in
 *          OpenGL it would generate the VBO specific to the model 3D and cache
 *          that value so it can be used later on
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include <stdint.h>
#include "Asset3D.hpp"
#include "OpenGL.h"

class OpenGLAsset3D : public Asset3D
{
  public:
    /**
     * Number of texture mipmaps that will be generated for each
     * texture
     *
     * TODO: Add this value to the global configuration of the engine
     */
    const uint32_t NumTexturesMipmaps = 4;

    /**
     * Prepares the asset for use with OpenGL drawing calls. It makes
     * use of the inherited asset 3D data to upload it to the GPU. Only
     * after calling prepare the rest of the methods can be used to render
     * this model
     *
     * @returns true or false
     */
    bool prepare();

    /**
     * Destroyes all allocated buffers and arrays in OpenGL. After this method
     * is called no other methods can be called except prepare()
     */
    bool destroy();

    /**
     * Retrieves the ID for the vertex array
     *
     * @return ID for the vertex array object
     */
    uint32_t getVertexArrayID() { return _gVAO; }

    /**
     * Returns the ID for the indices buffer object
     *
     * @return ID for the indices buffer object
     */
    uint32_t getIndicesArrayID() { return _indicesBO; }

    /**
     * Returns the vector of textures IDs associated with this
     * asset3D. The order must be the same as the order of the
     * textures vector in the base class
     *
     * @return vector of textures IDs
     */
    const std::vector<uint32_t> &getTexturesIDs() { return _texturesIDs; }

  private:

    GLuint _gVAO;                       /**< Vertex array object ID */
    GLuint _vertexDataVBO;              /**< Vertex buffer object ID */
    GLuint _indicesBO;                  /**< Indices buffer object ID */
    std::vector<uint32_t> _texturesIDs; /**< Textures ID vector */
};
