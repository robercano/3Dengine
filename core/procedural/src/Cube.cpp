#include <GL/glew.h>
#include <GL/glfw.h>
#include <string.h>
#include <glm/glm.hpp>
using namespace glm;
#include <string>

#include "Cube.hpp"
#include "Renderer.hpp"

using namespace procedural;

Cube::Cube()
{
    GLfloat verts[][3] = {{-1.0, -1.0, -1.0}, {-1.0, -1.0, 1.0}, {-1.0, 1.0, -1.0}, {-1.0, 1.0, 1.0},
                          {1.0, -1.0, -1.0},  {1.0, -1.0, 1.0},  {1.0, 1.0, -1.0},  {1.0, 1.0, 1.0}};

    GLfloat colors[][3] = {{0.0, 0.0, 0.0}, {0.0, 0.0, 1.0}, {0.0, 1.0, 0.0}, {0.0, 1.0, 1.0},
                           {1.0, 0.0, 0.0}, {1.0, 0.0, 0.0}, {1.0, 1.0, 0.0}, {1.0, 1.0, 1.0}};

    GLuint indices[] = {0, 1, 2, 2, 1, 3, 0, 2, 4, 4, 2, 6, 4, 6, 5, 5, 6, 7, 2, 3, 6, 6, 3, 7, 5, 7, 3, 5, 3, 1, 1, 4, 5, 4, 1, 0};

    _vertsArray = new GLfloat[sizeof verts];
    memcpy(_vertsArray, verts, sizeof verts);
    _vertsArrayLen = sizeof verts;

    _colorsArray = new GLfloat[sizeof colors];
    memcpy(_colorsArray, colors, sizeof colors);
    _colorsArrayLen = sizeof colors;

    _indicesArray = new GLuint[sizeof indices];
    memcpy(_indicesArray, indices, sizeof indices);
    _indicesArrayLen = sizeof indices;
}

Cube::~Cube()
{
    delete[] _vertsArray;
    delete[] _colorsArray;
    delete[] _indicesArray;
}

const GLfloat *Cube::getVerticesArray() { return _vertsArray; }
uint32_t Cube::getVerticesArrayLen() { return _vertsArrayLen; }
const GLfloat *Cube::getColorsArray() { return _colorsArray; }
uint32_t Cube::getColorsArrayLen() { return _colorsArrayLen; }
const GLuint *Cube::getIndicesArray() { return _indicesArray; }
uint32_t Cube::getIndicesArrayLen() { return _indicesArrayLen; }
