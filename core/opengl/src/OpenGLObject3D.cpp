/**
 * @class	OpenGLObject3D
 * @brief	OpengGL object 3D representation
 *
 * @author	Roberto Cano
 */

#include "OpenGLObject3D.hpp"
#include "OpenGL.h"

bool OpenGLObject3D::init(const Object3D &object)
{
	/* Generate a vertex array to reference the attributes */
	GL( glGenVertexArrays(1, &_gVAO) );
	GL( glBindVertexArray(_gVAO) );
    {
        /* Generate a buffer object for the vertices positions */
        GL( glGenBuffers(1, &_verticesVBO) );
        GL( glBindBuffer(GL_ARRAY_BUFFER, _verticesVBO) );
        {
            /* Upload the data for this buffer */
            GL( glBufferData(GL_ARRAY_BUFFER, object.getVerticesArrayLen() * sizeof(GLfloat), object.getVerticesArray(), GL_STATIC_DRAW) );

            /* Link the data with the first shader attribute */
            GL( glEnableVertexAttribArray(0) );
            GL( glVertexAttribPointer(
                    0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
                    3,                  // size
                    GL_FLOAT,           // type
                    GL_FALSE,           // normalized?
                    0,                  // stride
                    (void*)0            // array buffer offset
                    ) );
        }

        /* Generate a buffer for the vertices colors */
        GL( glGenBuffers(1, &_colorsVBO) );
        GL( glBindBuffer(GL_ARRAY_BUFFER, _colorsVBO) );
        {
            /* Upload the data for this buffer */
            GL( glBufferData(GL_ARRAY_BUFFER, object.getColorsArrayLen() * sizeof (GLfloat), object.getColorsArray(), GL_STATIC_DRAW) );

            /* Link the data with the second shader attribute */
            GL( glEnableVertexAttribArray(1) );
            GL( glVertexAttribPointer(
                    1,                  // attribute
                    3,                  // size
                    GL_FLOAT,           // type
                    GL_FALSE,           // normalized?
                    0,                  // stride
                    (void*)0            // array buffer offset
                    ) );
        }

        /* Generate a buffer for the vertices colors */
        GL( glGenBuffers(1, &_normalsVBO) );
        GL( glBindBuffer(GL_ARRAY_BUFFER, _normalsVBO) );
        {
            /* Upload the data for this buffer */
            GL( glBufferData(GL_ARRAY_BUFFER, object.getNormalsArrayLen() * sizeof (GLfloat), object.getNormalsArray(), GL_STATIC_DRAW) );

            /* Link the data with the second shader attribute */
            GL( glEnableVertexAttribArray(2) );
            GL( glVertexAttribPointer(
                    2,                  // attribute
                    3,                  // size
                    GL_FLOAT,           // type
                    GL_FALSE,           // normalized?
                    0,                  // stride
                    (void*)0            // array buffer offset
                    ) );
        }

        /* Generate the buffer for the indices */
        GL( glGenBuffers(1, &_indicesVBO) );
        GL( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indicesVBO) );
        {
            /* Upload the data */
            GL( glBufferData(GL_ELEMENT_ARRAY_BUFFER, object.getIndicesArrayLen() * sizeof(GLuint), object.getIndicesArray(), GL_STATIC_DRAW) );
        }

    }
    GL( glBindVertexArray(0) );

    _indicesLen = object.getIndicesArrayLen();

    return true;
}

bool OpenGLObject3D::destroy()
{
	GL( glDeleteBuffers(1, &_colorsVBO) );
	GL( glDeleteBuffers(1, &_verticesVBO) );
	GL( glDeleteBuffers(1, &_normalsVBO) );
	GL( glDeleteVertexArrays(1, &_gVAO) );
    return true;
}
