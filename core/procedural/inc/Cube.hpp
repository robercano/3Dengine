/**
 * @class	Cube
 * @brief   Procedural cube
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include "Object3D.hpp"

namespace procedural
{
    class Cube : public Object3D
    {
        public:
            Cube();
            ~Cube();

            /* Object3D methods */
            const GLfloat *getVerticesArray();
            uint32_t getVerticesArrayLen();
            const GLfloat *getColorsArray();
            uint32_t getColorsArrayLen();
            const GLuint *getIndicesArray();
            uint32_t getIndicesArrayLen();

        private:
            GLfloat *_vertsArray;
            uint32_t _vertsArrayLen;

            GLfloat *_colorsArray;
            uint32_t _colorsArrayLen;

            GLuint *_indicesArray;
            uint32_t _indicesArrayLen;
    };

} /* namespace procedural */
