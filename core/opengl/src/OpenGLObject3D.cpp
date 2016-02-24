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
    uint32_t offset;

	/* Generate a vertex array to reference the attributes */
	GL( glGenVertexArrays(1, &_gVAO) );
	GL( glBindVertexArray(_gVAO) );
    {
        /* Generate a buffer object for the vertices positions */
        GL( glGenBuffers(1, &_vertexDataVBO) );
        GL( glBindBuffer(GL_ARRAY_BUFFER, _vertexDataVBO) );
        {
            /* Upload the data for this buffer */
            GL( glBufferData(GL_ARRAY_BUFFER, object.getVertexDataSize(), object.getVertexData(), GL_STATIC_DRAW) );

            /* First attribute contains the vertex coordinates */
            GL( glEnableVertexAttribArray(0) );
            GL( glVertexAttribPointer(
                    0,                             // attribute. No particular reason for 0, but must match the layout in the shader.
                    3,                             // size
                    GL_FLOAT,                      // type
                    GL_FALSE,                      // normalized?
                    sizeof(Object3D::VertexData),// stride
                    (void*)0                       // array buffer offset
                    ) );

            /* Second attibute contains the normals */
            offset = 12;
            GL( glEnableVertexAttribArray(1) );
            GL( glVertexAttribPointer(
                    1,                             // attribute
                    3,                             // size
                    GL_FLOAT,                      // type
                    GL_FALSE,                      // normalized?
                    sizeof(Object3D::VertexData),  // stride
                    reinterpret_cast<void*>(offset)                  // array buffer offset
                    ) );

            /* Third attribute contains the UV coordinates */
            offset = 24;
            GL( glEnableVertexAttribArray(2) );
            GL( glVertexAttribPointer(
                    2,                             // attribute
                    2,                             // size
                    GL_FLOAT,                      // type
                    GL_FALSE,                      // normalized?
                    sizeof(Object3D::VertexData),// stride
                    reinterpret_cast<void*>(offset)                  // array buffer offset
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
	GL( glDeleteBuffers(1, &_vertexDataVBO) );
	GL( glDeleteVertexArrays(1, &_gVAO) );
    return true;
}
