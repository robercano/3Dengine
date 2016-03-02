/**
 * @class	OpenGLMaterial
 * @brief	OpenGL material implemented as a block uniform to be used in
 *          a shader. Contains ambient, diffuse, specular, alpha and shininess
 *          components
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#include <string.h>
#include "OpenGLMaterial.hpp"
#include <glm/glm.hpp>

OpenGLMaterial::~OpenGLMaterial()
{
    glDeleteBuffers(1, &_uniformBO);
    delete[] _paramsBuffer;
}

bool OpenGLMaterial::bindToShader(GLuint programID)
{
    const GLchar *names[] = { "Material.ambient", "Material.diffuse", "Material.specular", "Material.alpha", "Material.shininess" };
    //GLuint blockIndex;
    //GLint blockSize;
    GLuint indices[5];
    int i;

    GL( _blockIndex = glGetUniformBlockIndex(programID, "Material") );
    if (_blockIndex == GL_INVALID_INDEX) {
        return false;
    }
    GL( glGetActiveUniformBlockiv(programID, _blockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &_blockSize) );

    GL( glGetUniformIndices(programID, 5, names, indices) );
    for (i=0; i<5; ++i) {
        if (indices[i] == GL_INVALID_INDEX) {
            return false;
        }
    }
    GL( glGetActiveUniformsiv(programID, 5, indices, GL_UNIFORM_OFFSET, _paramsOffset) );

    _paramsBuffer = new GLubyte[_blockSize];

    GL( glGenBuffers(1, &_uniformBO) );
    GL( glBindBuffer(GL_UNIFORM_BUFFER, _uniformBO) );
    GL( glBufferData(GL_UNIFORM_BUFFER, _blockSize, NULL, GL_DYNAMIC_DRAW) );
    GL( glBindBufferBase(GL_UNIFORM_BUFFER, _blockIndex, _uniformBO) );

    return true;
}

bool OpenGLMaterial::setValues(glm::vec3 &ambient, glm::vec3 &diffuse, glm::vec3 &specular, float alpha, float shininess)
{
    memcpy(_paramsBuffer + _paramsOffset[0], &ambient[0],  sizeof ambient);
    memcpy(_paramsBuffer + _paramsOffset[1], &diffuse[0],  sizeof diffuse);
    memcpy(_paramsBuffer + _paramsOffset[2], &specular[0], sizeof specular );
    memcpy(_paramsBuffer + _paramsOffset[3], &alpha,       sizeof alpha);
    memcpy(_paramsBuffer + _paramsOffset[4], &shininess,   sizeof shininess);

    GL( glBindBuffer(GL_UNIFORM_BUFFER, _uniformBO) );
    GL( glBufferSubData(GL_UNIFORM_BUFFER, 0, _blockSize, _paramsBuffer) );
    //GL( glBindBufferBase(GL_UNIFORM_BUFFER, _blockIndex, _uniformBO) );

    return true;
}
