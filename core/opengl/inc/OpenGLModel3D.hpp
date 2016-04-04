/**
 * @class	RendererModel3D
 * @brief	Object 3D representation prepared for the specific renderer
 *          This is a composite model generated through Renderer::prepareModel3D()
 *          that prepares the model for the specific renderer. For example in
 *          OpenGL it would generate the VBO specific to the model 3D and cache
 *          that value so it can be used later on
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include <stdint.h>
#include "RendererModel3D.hpp"
#include "OpenGL.h"

class OpenGLModel3D : public RendererModel3D
{
	public:
        bool init(const Model3D &model);
        bool destroy();
        uint32_t getVertexArrayID() { return _gVAO; }
        uint32_t getIndicesArrayID() { return _indicesBO; }
        const std::vector<Material> &getMaterials() { return _materials; }
        const std::vector<uint32_t> &getTextures() { return _texturesIDs; }
        const std::vector<uint32_t> &getIndicesOffsets() { return _indicesOffsets; }
        const std::vector< uint32_t > &getIndicesCount() const { return _indicesCount; }

    private:
        GLuint _gVAO;
        GLuint _vertexDataVBO;
        GLuint _indicesBO;
        std::vector<Material> _materials;
        std::vector<uint32_t> _texturesIDs;
        std::vector<uint32_t> _indicesOffsets;
        std::vector<uint32_t> _indicesCount;
};
