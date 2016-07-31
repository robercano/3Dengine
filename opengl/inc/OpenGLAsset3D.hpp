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

    uint32_t getVertexArrayID() { return _gVAO; }
    uint32_t getIndicesArrayID() { return _indicesBO; }
    const std::vector<uint32_t> &getTexturesIDs() { return _texturesIDs; }
  private:
    GLuint _gVAO;
    GLuint _vertexDataVBO;
    GLuint _indicesBO;
    std::vector<uint32_t> _texturesIDs;
};
