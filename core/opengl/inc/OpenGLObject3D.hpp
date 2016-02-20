/**
 * @class	RendererObject3D
 * @brief	Object 3D representation prepared for the specific renderer
 *          This is a composite object generated through Renderer::prepareObject3D()
 *          that prepares the object for the specific renderer. For example in
 *          OpenGL it would generate the VBO specific to the object 3D and cache
 *          that value so it can be used later on
 *
 * @author	Roberto Sosa Cano
 */
#pragma once

#include "RendererObject3D.hpp"
#include <stdint.h>

class OpenGLObject3D : public RendererObject3D
{
	public:
        bool init(const Object3D &object);
        bool destroy();
        const uint32_t getVertexArrayID() { return _gVAO; }
        const uint32_t getIndicesArrayLen() { return _indicesLen; }

    private:
        /**
         * Vertex array object ID
         */
        uint32_t _gVAO;

        /**
         * Vertex buffer object for vertices
         */
        uint32_t _verticesVBO;

        /**
         * Vertex buffer object for colors
         */
        uint32_t _colorsVBO;

        /**
         * Vertex buffer object for normals
         */
        uint32_t _normalsVBO;

        /**
         * Vertex buffer object for indices
         */
        uint32_t _indicesVBO;

        uint32_t _indicesLen;
};
