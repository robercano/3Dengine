/**
 * @class	Cube
 * @brief   Procedural cube
 *
 * @author	Roberto Sosa Cano
 */
#pragma once

#include "Object3D.hpp"

namespace procedural
{
    class Cube : public Object3D
    {
        public:
            /** Object3D inherited methods */
            bool init();
            bool destroy();
            uint32_t getVertexArrayIndex();

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
             * Vertex buffer object for indices
             */
            uint32_t _indicesVBO;
    };

} /* namespace procedural */
