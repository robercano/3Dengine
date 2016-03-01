/**
 * @class	OpenGLShaderMaterial
 * @brief	Class to abstract a shader material. A material
 *          contains ambient, diffuse, specular, alpha and shininess
 *          components
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include "OpenGLShaderMaterial.hpp"

OpenGLShaderMaterial::~OpenGLShaderMaterial()
{
    glDeleteBuffers(_uniformBO);
}

bool bindToProgram(GLuint programID)
{
    GLuint _uniformBO;
    GLuint blockIndex = glGetUniformBlockIndex(programHandle,
            "BlobSettings");

    glGetActiveUniformBlockiv(programHandle, blockIndex,
            GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);

    GLubyte * blockBuffer= (GLubyte *) malloc(blockSize);

    // Query for the offsets of each block variable
    const GLchar *names[] = { "InnerColor", "OuterColor", "RadiusInner", "RadiusOuter" };

    GLuint indices[4];
    glGetUniformIndices(programHandle, 4, names, indices);

    GLint offset[4];
    glGetActiveUniformsiv(programHandle, 4, indices,
            GL_UNIFORM_OFFSET, offset);

    Place the data into the buffer at the appropriate offsets.
        GLfloat outerColor[] = {0.0f, 0.0f, 0.0f, 0.0f};
    GLfloat innerColor[] = {1.0f, 1.0f, 0.75f, 1.0f};
    GLfloat innerRadius = 0.25f, outerRadius = 0.45f;

    memcpy(blockBuffer + offset[0], innerColor,
            4 * sizeof(GLfloat));
    memcpy(blockBuffer + offset[1], outerColor,
            4 * sizeof(GLfloat));
    memcpy(blockBuffer + offset[2], &innerRadius,
            sizeof(GLfloat));
    memcpy(blockBuffer + offset[3], &outerRadius,
            sizeof(GLfloat));

    glGenBuffers( 1, &_uniformBO );
    glBindBuffer( GL_UNIFORM_BUFFER, _uniformBO );
    glBufferData( GL_UNIFORM_BUFFER, blockSize, blockBuffer,
            GL_DYNAMIC_DRAW );

    glBindBufferBase( GL_UNIFORM_BUFFER, blockIndex, _uniformBO );
}
