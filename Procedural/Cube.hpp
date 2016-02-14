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
            Cube();
            ~Cube();

            /* Object3D methods */
            const GLfloat *getVerticesArray();
            uint32_t getVerticesArrayLen();
            const GLfloat *getColorsArray();
            uint32_t getColorsArrayLen();
            const GLubyte *getIndicesArray();
            uint32_t getIndicesArrayLen();

        private:
            GLfloat *_vertsArray;
            uint32_t _vertsArrayLen;

            GLfloat *_colorsArray;
            uint32_t _colorsArrayLen;

            GLubyte *_indicesArray;
            uint32_t _indicesArrayLen;
    };

} /* namespace procedural */
